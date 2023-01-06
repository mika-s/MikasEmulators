#include "zxspectrum_48k_session.h"
#include "audio.h"
#include "chips/z80/cpu.h"
#include "chips/z80/disassembler.h"
#include "chips/z80/interrupt_mode.h"
#include "chips/z80/manual_state.h"
#include "cpu_io.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"
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
#include <iosfwd>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace emu::applications::zxspectrum_48k {
class Settings;
}

namespace emu::applications::zxspectrum_48k {

using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::IoDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::util::byte::high_byte;
using emu::util::byte::is_bit_set;
using emu::util::byte::to_u16;
using emu::util::string::hexify;
using emu::util::string::split;
using emu::z80::Disassembler;
using emu::z80::InterruptMode;

ZxSpectrum48kSession::ZxSpectrum48kSession(
    [[maybe_unused]] Settings const& settings,
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

ZxSpectrum48kSession::ZxSpectrum48kSession(
    Settings const& settings,
    bool is_starting_paused,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    EmulatorMemory<u16, u8>& memory,
    ManualState initial_cpu_state)
    : ZxSpectrum48kSession(settings, is_starting_paused, std::move(gui), std::move(input), memory)
{
    m_cpu->set_state_manually(initial_cpu_state);
}

ZxSpectrum48kSession::~ZxSpectrum48kSession()
{
    m_gui->remove_gui_observer(this);
    m_input->remove_io_observer(this);
    m_cpu->remove_in_observer(this);
    m_cpu->remove_out_observer(this);
}

void ZxSpectrum48kSession::run()
{
    m_cpu->start();

    cyc cycles;

    while (!m_state_context->current_state()->is_exit_state()) {
        m_state_context->current_state()->perform(cycles);
    }
}

void ZxSpectrum48kSession::pause()
{
    m_state_context->change_state(m_state_context->paused_state());
}

void ZxSpectrum48kSession::stop()
{
    m_state_context->change_state(m_state_context->stopped_state());
}

void ZxSpectrum48kSession::setup_cpu()
{
    const u16 initial_pc = 0;

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
    m_cpu->add_in_observer(*this);
}

void ZxSpectrum48kSession::setup_debugging()
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
            { "u", 5 },
            { "a", 4 },
            { "u", 3 },
            { "p", 2 },
            { "u", 1 },
            { "c", 0 } }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "0xfe",
        [&]() { return m_outputs_during_cycle.contains(s_port_0xfe); },
        [&]() { return m_outputs_during_cycle[s_port_0xfe]; }));
    m_debug_container->add_io(IoDebugContainer<u8>(
        "LAST-K",
        [&]() { return true; },
        [&]() { return memory().at(0x5c08); }));
    m_debug_container->add_memory(MemoryDebugContainer<u8>(
        [&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());

    m_gui->attach_debugger(m_debugger);
    m_gui->attach_debug_container(m_debug_container);
    m_gui->attach_cpu_io(&m_cpu_io);
    m_gui->attach_logger(m_logger);
}

void ZxSpectrum48kSession::gui_request(GuiRequest request)
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

void ZxSpectrum48kSession::in_requested(u16 port)
{
    // Should take 12.5 cycles instead of 11, when accessing 0xFE.
    if (!is_bit_set(port, 0)) {
        m_cpu->input(port, m_cpu_io.keyboard_input(port));
    }
}

void ZxSpectrum48kSession::out_changed(u16 port)
{
    if (!m_outputs_during_cycle.contains(port)) {
        m_outputs_during_cycle[port] = m_cpu->a();
    } else {
        m_outputs_during_cycle[port] |= m_cpu->a();
    }

    switch (port) { // NOLINT
    case s_port_0xfe:
        m_cpu_io.m_out_port0xfe = m_cpu->a();
        if (is_bit_set(m_cpu_io.m_out_port0xfe, s_mic_bit)) {
            // MIC out
        }
        if (is_bit_set(m_cpu_io.m_out_port0xfe, s_beep_bit)) {
            m_audio.beep();
        }
        break;
    default:
        throw std::runtime_error("Illegal output port for ZX Spectrum 48k");
    }
}

void ZxSpectrum48kSession::key_pressed(KeyRequest request)
{
    switch (request) {
    case TOGGLE_MUTE:
        m_audio.toggle_mute();
        break;
    default:
        break;
    }
}

std::vector<u8> ZxSpectrum48kSession::memory()
{
    return { m_memory.begin(), m_memory.end() };
}

std::vector<DisassembledLine<u16, 16>> ZxSpectrum48kSession::disassemble_program()
{
    EmulatorMemory<u16, u8> sliced_for_disassembly = m_memory.slice(0, 0x4000);

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
