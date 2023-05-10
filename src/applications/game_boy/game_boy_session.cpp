#include "game_boy_session.h"
#include "audio.h"
#include "chips/lr35902/cpu.h"
#include "chips/lr35902/disassembler.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/string_util.h"
#include "gui.h"
#include "gui_request.h"
#include "interfaces/input.h"
#include "interfaces/state.h"
#include "key_request.h"
#include "states/paused_state.h"
#include "states/running_state.h"
#include "states/state_context.h"
#include "states/stepping_state.h"
#include "states/stopped_state.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace emu::applications::game_boy {

using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::IoDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::util::string::split;
using emu::lr35902::Disassembler;

GameBoySession::GameBoySession(
    bool is_starting_paused,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    std::shared_ptr<Audio> audio,
    std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io,
    EmulatorMemory<u16, u8>& memory)
    : m_memory_mapped_io(std::move(memory_mapped_io))
    , m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_audio(std::move(audio))
    , m_memory(memory)
    , m_logger(std::make_shared<Logger>())
    , m_debugger(std::make_shared<Debugger<u16, 16>>())
{
    setup_cpu();
    setup_debugging();

    m_gui->add_gui_observer(*this);
    m_input->add_io_observer(*this);

    m_state_context = std::make_shared<StateContext>(
        m_gui_io,
        m_gui,
        m_input,
        m_audio,
        m_cpu,
        m_memory,
        m_memory_mapped_io,
        m_vblank_interrupt_return,
        m_logger,
        m_debugger,
        m_debug_container,
        m_outputs_during_cycle,
        m_governor,
        m_is_in_debug_mode);
    m_state_context->set_running_state(std::make_shared<RunningState>(m_state_context));
    m_state_context->set_paused_state(std::make_shared<PausedState>(m_state_context));
    m_state_context->set_stepping_state(std::make_shared<SteppingState>(m_state_context));
    m_state_context->set_stopped_state(std::make_shared<StoppedState>(m_state_context));

    if (is_starting_paused) {
        m_state_context->change_state(m_state_context->paused_state());
    } else {
        m_state_context->change_state(m_state_context->running_state());
    }
}

GameBoySession::~GameBoySession()
{
    m_gui->remove_gui_observer(this);
    m_input->remove_io_observer(this);
    m_cpu->remove_out_observer(this);
}

void GameBoySession::run()
{
    m_cpu->start();

    cyc cycles;

    while (!m_state_context->current_state()->is_exit_state()) {
        m_state_context->current_state()->perform(cycles);
    }
}

void GameBoySession::pause()
{
    m_state_context->change_state(m_state_context->paused_state());
}

void GameBoySession::stop()
{
    m_state_context->change_state(m_state_context->stopped_state());
}

void GameBoySession::setup_cpu()
{
    const u16 initial_pc = 0;

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
}

void GameBoySession::setup_debugging()
{
    m_debug_container = std::make_shared<DebugContainer<u16, u8, 16>>();
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "A",
        [&]() { return m_cpu->a(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "B",
        [&]() { return m_cpu->b(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "C",
        [&]() { return m_cpu->c(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "D",
        [&]() { return m_cpu->d(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "E",
        [&]() { return m_cpu->e(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "H",
        [&]() { return m_cpu->h(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "L",
        [&]() { return m_cpu->l(); }));
    m_debug_container->add_pc([&]() { return m_cpu->pc(); });
    m_debug_container->add_sp([&]() { return m_cpu->sp(); });
    m_debug_container->add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
    m_debug_container->add_flag_register(FlagRegisterDebugContainer<u8>(
        "F",
        [&]() { return m_cpu->f(); },
        { { "z", 7 },
            { "n", 6 },
            { "h", 5 },
            { "c", 4 },
            { "0", 3 },
            { "0", 2 },
            { "0", 1 },
            { "0", 0 } }));
    m_debug_container->add_memory(MemoryDebugContainer<u8>(
        [&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());
//    m_debug_container->add_tilemap(m_gui->tiles());
//    m_debug_container->add_spritemap(m_gui->sprites());
//    m_debug_container->add_waveforms(m_audio->waveforms());

    m_gui->attach_debugger(m_debugger);
    m_gui->attach_debug_container(m_debug_container);
    m_gui->attach_logger(m_logger);
}

void GameBoySession::gui_request(GuiRequest request)
{
    switch (request.m_type) {
    case RUN:
        m_state_context->change_state(m_state_context->running_state());
        break;
    case PAUSE:
        m_state_context->change_state(m_state_context->paused_state());
        break;
    case STOP:
        m_state_context->change_state(m_state_context->stopped_state());
        break;
    case DEBUG_MODE:
        m_is_in_debug_mode = request.m_payload;
        break;
    }
}

void GameBoySession::out_changed(u16 port)
{
    if (!m_outputs_during_cycle.contains(port)) {
        m_outputs_during_cycle[port] = m_cpu->a();
    } else {
        m_outputs_during_cycle[port] |= m_cpu->a();
    }

    if (port == s_out_port_vblank_interrupt_return) {
        m_vblank_interrupt_return = m_cpu->a();
    } else {
        throw std::runtime_error("Illegal output port for GameBoy");
    }
}

void GameBoySession::key_pressed(IoRequest request)
{
    switch (request) {
    case TOGGLE_MUTE:
        m_audio->toggle_mute();
        break;
    case TOGGLE_TILE_DEBUG:
        m_gui->toggle_tile_debug();
        break;
    case TOGGLE_SPRITE_DEBUG:
        m_gui->toggle_sprite_debug();
        break;
    default:
        break;
    }
}

std::vector<u8> GameBoySession::memory()
{
    return { m_memory.begin(), m_memory.end() };
}

std::vector<DisassembledLine<u16, 16>> GameBoySession::disassemble_program()
{
    EmulatorMemory<u16, u8> sliced_for_disassembly = m_memory.slice(0, 0x3fff);

    std::stringstream ss;
    Disassembler disassembler(sliced_for_disassembly, ss);
    disassembler.disassemble();

    std::vector<std::string> disassembled_program = split(ss, "\n");

    disassembled_program.erase(
        std::remove_if(disassembled_program.begin(), disassembled_program.end(), [](std::string const& s) { return s.empty(); }));

    std::vector<DisassembledLine<u16, 16>> lines;
    std::transform(disassembled_program.begin(), disassembled_program.end(), std::back_inserter(lines),
        [](std::string const& line) { return DisassembledLine<u16, 16>(line); });

    return lines;
}
}
