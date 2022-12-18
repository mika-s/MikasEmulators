#include "space_invaders_session.h"
#include "audio.h"
#include "chips/8080/cpu.h"
#include "chips/8080/disassembler.h"
#include "chips/8080/shift_register.h"
#include "cpu_io.h"
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
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace emu::applications::space_invaders {
class Settings;
}

namespace emu::applications::space_invaders {

using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::IoDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::i8080::Disassembler;
using emu::util::string::split;

SpaceInvadersSession::SpaceInvadersSession(
    Settings const& settings,
    bool is_starting_paused,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    EmulatorMemory<u16, u8>& memory)
    : m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_memory(memory)
    , m_logger(std::make_shared<Logger>())
    , m_debugger(std::make_shared<Debugger<u16, 16>>())
{
    setup_cpu();
    setup_debugging();
    m_cpu_io.set_dipswitches(settings);

    m_gui->add_gui_observer(*this);
    m_input->add_io_observer(*this);

    m_state_context = std::make_shared<StateContext>(
        m_cpu_io,
        m_gui_io,
        m_gui,
        m_input,
        m_cpu,
        m_memory,
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

SpaceInvadersSession::~SpaceInvadersSession()
{
    m_gui->remove_gui_observer(this);
    m_input->remove_io_observer(this);
    m_cpu->remove_in_observer(this);
    m_cpu->remove_out_observer(this);
}

void SpaceInvadersSession::run()
{
    m_cpu->start();

    cyc cycles;

    while (!m_state_context->current_state()->is_exit_state()) {
        m_state_context->current_state()->perform(cycles);
    }
}

void SpaceInvadersSession::pause()
{
    m_state_context->change_state(m_state_context->paused_state());
}

void SpaceInvadersSession::stop()
{
    m_state_context->change_state(m_state_context->stopped_state());
}

void SpaceInvadersSession::setup_cpu()
{
    const u16 initial_pc = 0;

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
    m_cpu->add_in_observer(*this);
}

void SpaceInvadersSession::setup_debugging()
{
    m_debug_container = std::make_shared<DebugContainer<u16, u8, 16>>();
    m_debug_container->add_register(RegisterDebugContainer<u8>("A", [&]() { return m_cpu->a(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("B", [&]() { return m_cpu->b(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("C", [&]() { return m_cpu->c(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("D", [&]() { return m_cpu->d(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("E", [&]() { return m_cpu->e(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("H", [&]() { return m_cpu->h(); }));
    m_debug_container->add_register(RegisterDebugContainer<u8>("L", [&]() { return m_cpu->l(); }));
    m_debug_container->add_pc([&]() { return m_cpu->pc(); });
    m_debug_container->add_sp([&]() { return m_cpu->sp(); });
    m_debug_container->add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
    m_debug_container->add_flag_register(FlagRegisterDebugContainer<u8>(
        "F",
        [&]() { return m_cpu->f(); },
        { { "s", 7 },
            { "z", 6 },
            { "u", 5 },
            { "a", 4 },
            { "u", 3 },
            { "p", 2 },
            { "u", 1 },
            { "c", 0 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "shift (change offset)",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_shift_offset); },
        [&]() { return m_outputs_during_cycle[s_out_port_shift_offset]; }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "shift (do shift)",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_do_shift); },
        [&]() { return m_outputs_during_cycle[s_out_port_do_shift]; }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "watchdog",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_watchdog); },
        [&]() { return m_outputs_during_cycle[s_out_port_watchdog]; }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "out sound 1",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_sound_1); },
        [&]() { return m_outputs_during_cycle[s_out_port_sound_1]; },
        { { "ufo", 0 },
            { "shot", 1 },
            { "flash", 2 },
            { "invader_die", 3 },
            { "extended_play", 4 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "out sound 2",
        [&]() { return m_outputs_during_cycle.contains(s_out_port_sound_2); },
        [&]() { return m_outputs_during_cycle[s_out_port_sound_2]; },
        { { "fleet_movement_1", 0 },
            { "fleet_movement_2", 1 },
            { "fleet_movement_3", 2 },
            { "fleet_movement_4", 3 },
            { "ufo_hit", 4 } }));
    m_debug_container->add_memory(MemoryDebugContainer<u8>(
        [&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());

    m_gui->attach_debugger(m_debugger);
    m_gui->attach_debug_container(m_debug_container);
    m_gui->attach_logger(m_logger);
}

void SpaceInvadersSession::gui_request(GuiRequest request)
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

void SpaceInvadersSession::in_requested(u8 port)
{
    switch (port) {
    case s_in_port_unused:
        m_cpu->input(s_in_port_unused, m_cpu_io.m_in_port0);
        break;
    case s_in_port_1:
        m_cpu->input(s_in_port_1, m_cpu_io.m_in_port1);
        break;
    case s_in_port_2:
        m_cpu->input(s_in_port_2, m_cpu_io.m_in_port2);
        break;
    case s_in_port_read_shift:
        m_cpu->input(s_in_port_read_shift, m_cpu_io.m_shift_register.read());
        break;
    default:
        throw std::runtime_error("Illegal input port for Space Invaders");
    }
}

void SpaceInvadersSession::out_changed(u8 port)
{
    if (!m_outputs_during_cycle.contains(port)) {
        m_outputs_during_cycle[port] = m_cpu->a();
    } else {
        m_outputs_during_cycle[port] |= m_cpu->a();
    }

    switch (port) {
    case s_out_port_shift_offset:
        m_cpu_io.m_shift_register.change_offset(m_cpu->a());
        break;
    case s_out_port_sound_1:
        m_audio.play_sound_port_1(m_cpu->a());
        break;
    case s_out_port_do_shift:
        m_cpu_io.m_shift_register.shift(m_cpu->a());
        break;
    case s_out_port_sound_2:
        m_audio.play_sound_port_2(m_cpu->a());
        break;
    case s_out_port_watchdog:
        break;
    default:
        throw std::runtime_error("Illegal output port for Space Invaders");
    }
}

void SpaceInvadersSession::key_pressed(KeyRequest request)
{
    switch (request) {
    case TOGGLE_MUTE:
        m_audio.toggle_mute();
        break;
    default:
        break;
    }
}

std::vector<u8> SpaceInvadersSession::memory()
{
    return { m_memory.begin(), m_memory.begin() + 0x3fff + 1 };
}

std::vector<DisassembledLine<u16, 16>> SpaceInvadersSession::disassemble_program()
{
    EmulatorMemory<u16, u8> sliced_for_disassembly = m_memory.slice(0, 0x2000);

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
