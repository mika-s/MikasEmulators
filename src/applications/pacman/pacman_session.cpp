#include "pacman_session.h"
#include "chips/z80/disassembler.h"
#include "chips/z80/interrupt_mode.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/string_util.h"
#include "interfaces/state.h"
#include "pacman/audio.h"
#include "pacman/gui.h"
#include "pacman/interfaces/input.h"
#include "pacman/memory_mapped_io_for_pacman.h"
#include "states/paused_state.h"
#include "states/running_state.h"
#include "states/stepping_state.h"
#include "states/stopped_state.h"
#include "z80/cpu.h"
#include "z80/interfaces/gui_observer.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace emu::applications::pacman {

using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::IoDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::util::string::split;
using emu::z80::Disassembler;
using emu::z80::InterruptMode;
using emu::z80::RunStatus::FINISHED;
using emu::z80::RunStatus::NOT_RUNNING;
using emu::z80::RunStatus::PAUSED;
using emu::z80::RunStatus::RUNNING;
using emu::z80::RunStatus::STEPPING;

PacmanSession::PacmanSession(
    const RunStatus startup_runstatus,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    std::shared_ptr<Audio> audio,
    std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
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

    m_running_state = std::make_shared<RunningState>(
        *this,
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
    m_paused_state = std::make_shared<PausedState>(
        *this,
        m_gui_io,
        m_gui,
        m_input,
        m_memory,
        m_memory_mapped_io,
        m_governor);
    m_stepping_state = std::make_shared<SteppingState>(
        *this,
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
        m_outputs_during_cycle);
    m_stopped_state = std::make_shared<StoppedState>(*this);

    if (startup_runstatus == PAUSED) {
        m_current_state = m_paused_state;
    } else {
        m_current_state = m_running_state;
    }
}

PacmanSession::~PacmanSession()
{
    m_gui->remove_gui_observer(this);
    m_input->remove_io_observer(this);
    m_cpu->remove_out_observer(this);
}

void PacmanSession::run()
{
    if (m_run_status == FINISHED) {
        throw std::invalid_argument("Programming error: the session has finished and cannot be run one more time");
    }

    m_cpu->start();

    cyc cycles;

    while (!m_current_state->is_exit_state()) {
        m_current_state->perform(cycles);
    }

    m_run_status = FINISHED;
}

void PacmanSession::pause()
{
    m_run_status = PAUSED;
}

void PacmanSession::stop()
{
    m_run_status = FINISHED;
}

void PacmanSession::setup_cpu()
{
    const u16 initial_pc = 0;

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
}

void PacmanSession::setup_debugging()
{
    m_debug_container = std::make_shared<DebugContainer<u16, u8, 16>>();
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "A",
        [&]() { return m_cpu->a(); },
        [&]() { return m_cpu->a_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "B",
        [&]() { return m_cpu->b(); },
        [&]() { return m_cpu->b_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "C",
        [&]() { return m_cpu->c(); },
        [&]() { return m_cpu->c_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "D",
        [&]() { return m_cpu->d(); },
        [&]() { return m_cpu->d_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "E",
        [&]() { return m_cpu->e(); },
        [&]() { return m_cpu->e_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "H",
        [&]() { return m_cpu->h(); },
        [&]() { return m_cpu->h_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>(
        "L",
        [&]() { return m_cpu->l(); },
        [&]() { return m_cpu->l_p(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("I", [&]() { return m_cpu->i(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("R", [&]() { return m_cpu->r(); }));
    m_debug_container->add_pc([&]() { return m_cpu->pc(); });
    m_debug_container->add_sp([&]() { return m_cpu->sp(); });
    m_debug_container->add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
    m_debug_container->add_interrupt_mode([&]() {
        switch (m_cpu->interrupt_mode()) {
        case InterruptMode::ZERO:
            return "0";
        case InterruptMode::ONE:
            return "1";
        case InterruptMode::TWO:
            return "2";
        default:
            throw std::invalid_argument("Unhandled InterruptMode");
        }
    });
    m_debug_container->add_flag_register(FlagRegisterDebugContainer<u8>(
        "F",
        [&]() { return m_cpu->f(); },
        { { "s", 7 },
            { "z", 6 },
            { "y", 5 },
            { "h", 4 },
            { "x", 3 },
            { "p", 2 },
            { "n", 1 },
            { "c", 0 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "vblank return",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_vblank_interrupt_return); },
        [&]() { return m_outputs_during_cycle[s_out_port_vblank_interrupt_return]; }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "coin counter",
        [&]() { return true; },
        [&]() { return m_memory_mapped_io->coin_counter(); }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "dipswitches",
        [&]() { return true; },
        [&]() { return m_memory_mapped_io->dipswitches(); },
        { { "coin 1 (AH)", 0 },
            { "coin 2 (AH)", 1 },
            { "lives 1 (AH)", 2 },
            { "lives 2 (AH)", 3 },
            { "bonus life 1 (AH)", 4 },
            { "bonus life 2 (AH)", 5 },
            { "difficulty (AH)", 6 },
            { "ghost names (AH)", 7 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "in 0",
        [&]() { return true; },
        [&]() { return m_memory_mapped_io->in0_read(); },
        { { "up (AL)", 0 },
            { "left (AL)", 1 },
            { "right (AL)", 2 },
            { "down (AL)", 3 },
            { "off (AH)", 4 },
            { "coin 1 (AL)", 5 },
            { "coin 2 (AL)", 6 },
            { "service 1 (AL)", 7 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "in 1",
        [&]() { return true; },
        [&]() { return m_memory_mapped_io->in1_read(); },
        { { "up (AL)", 0 },
            { "left (AL)", 1 },
            { "right (AL)", 2 },
            { "down (AL)", 3 },
            { "board test (AL)", 4 },
            { "start 1 (AL)", 5 },
            { "start 2 (AL)", 6 },
            { "cocktail (AL)", 7 } }));
    m_debug_container->add_memory(MemoryDebugContainer<u8>(
        [&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());
    m_debug_container->add_tilemap(m_gui->tiles());
    m_debug_container->add_spritemap(m_gui->sprites());
    m_debug_container->add_waveforms(m_audio->waveforms());

    m_gui->attach_debugger(m_debugger);
    m_gui->attach_debug_container(m_debug_container);
    m_gui->attach_logger(m_logger);
}

void PacmanSession::run_status_changed(RunStatus new_status)
{
    m_run_status = new_status;
    switch (m_run_status) {
    case NOT_RUNNING:
        change_state(stopped_state());
        break;
    case RUNNING:
        change_state(running_state());
        break;
    case PAUSED:
        change_state(paused_state());
        break;
    case FINISHED:
        change_state(stopped_state());
        break;
    case STEPPING:
        break;
    }
}

void PacmanSession::debug_mode_changed(bool is_in_debug_mode)
{
    m_is_in_debug_mode = is_in_debug_mode;
}

void PacmanSession::out_changed(u8 port)
{
    if (!m_outputs_during_cycle.contains(port)) {
        m_outputs_during_cycle[port] = m_cpu->a();
    } else {
        m_outputs_during_cycle[port] |= m_cpu->a();
    }

    if (port == s_out_port_vblank_interrupt_return) {
        m_vblank_interrupt_return = m_cpu->a();
    } else {
        throw std::runtime_error("Illegal output port for Pacman");
    }
}

void PacmanSession::io_changed(IoRequest request)
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

void PacmanSession::change_state(std::shared_ptr<State> new_state)
{
    m_current_state = new_state;
}

std::shared_ptr<State> PacmanSession::paused_state()
{
    return m_paused_state;
}

std::shared_ptr<State> PacmanSession::running_state()
{
    return m_running_state;
}

std::shared_ptr<State> PacmanSession::stepping_state()
{
    return m_stepping_state;
}

std::shared_ptr<State> PacmanSession::stopped_state()
{
    return m_stopped_state;
}

std::shared_ptr<State> PacmanSession::current_state()
{
    return m_current_state;
}

std::vector<u8> PacmanSession::memory()
{
    return { m_memory.begin(), m_memory.end() };
}

std::vector<DisassembledLine<u16, 16>> PacmanSession::disassemble_program()
{
    EmulatorMemory<u16, u8> sliced_for_disassembly = m_memory.slice(0, 0x3fff);

    std::stringstream ss;
    Disassembler disassembler(sliced_for_disassembly, ss);
    disassembler.disassemble();

    std::vector<std::string> disassembled_program = split(ss, "\n");

    disassembled_program.erase(
        std::remove_if(disassembled_program.begin(), disassembled_program.end(), [](std::string const& s) { return s.empty(); }));

    std::vector<DisassembledLine<u16, 16>> lines;
    std::transform(
        disassembled_program.begin(),
        disassembled_program.end(),
        std::back_inserter(lines),
        [](std::string const& line) { return DisassembledLine<u16, 16>(line); });

    return lines;
}
}
