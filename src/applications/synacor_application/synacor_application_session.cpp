#include "synacor_application_session.h"
#include "chips/trivial/synacor/cpu.h"
#include "chips/trivial/synacor/disassembler.h"
#include "chips/trivial/synacor/usings.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "gui_request.h"
#include "interfaces/state.h"
#include "states/paused_state.h"
#include "states/running_awaiting_input_state.h"
#include "states/running_state.h"
#include "states/state_context.h"
#include "states/stepping_state.h"
#include "states/stopped_state.h"
#include "ui.h"
#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <tuple>
#include <utility>
#include <vector>

namespace emu::applications::synacor {

using emu::debugger::DisassembledLine;
using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::synacor::Address;
using emu::synacor::Data;
using emu::synacor::Disassembler;
using emu::synacor::RawData;
using emu::util::byte::to_u16;
using emu::util::string::split;

SynacorApplicationSession::SynacorApplicationSession(
    bool is_only_run_once,
    bool is_starting_paused,
    std::shared_ptr<Ui> gui,
    std::shared_ptr<Input> input,
    std::string loaded_file,
    std::string file_content,
    EmulatorMemory<Address, RawData> memory)
    : m_is_only_run_once(is_only_run_once)
    , m_ui(std::move(gui))
    , m_input(std::move(input))
    , m_memory(std::move(memory))
    , m_loaded_file(std::move(loaded_file))
    , m_file_content(std::move(file_content))
    , m_logger(std::make_shared<Logger>())
    , m_debugger(std::make_shared<Debugger<Address, 16>>())
{
    setup_cpu();
    setup_debugging();

    m_ui->add_ui_observer(*this);

    m_state_context = std::make_shared<StateContext>(
        m_gui_io,
        m_ui,
        m_input,
        m_cpu,
        m_memory,
        m_logger,
        m_debugger,
        m_debug_container,
        m_governor,
        m_is_only_run_once,
        m_is_awaiting_input,
        m_is_in_debug_mode);
    m_state_context->set_running_state(std::make_shared<RunningState>(m_state_context));
    m_state_context->set_running_awaiting_input_state(std::make_shared<RunningAwaitingInputState>(m_state_context));
    m_state_context->set_paused_state(std::make_shared<PausedState>(m_state_context));
    m_state_context->set_stepping_state(std::make_shared<SteppingState>(m_state_context));
    m_state_context->set_stopped_state(std::make_shared<StoppedState>(m_state_context));

    if (is_starting_paused) {
        m_state_context->change_state(m_state_context->paused_state());
    } else {
        m_state_context->change_state(m_state_context->running_state());
    }
}

SynacorApplicationSession::~SynacorApplicationSession()
{
    m_ui->remove_ui_observer(this);
    m_cpu->remove_out_observer(this);
    m_cpu->remove_in_observer(this);
}

std::function<void()> loop;
void main_loop()
{
    loop();
}

void SynacorApplicationSession::run()
{
    m_cpu->start();

    cyc cycles;

    while (!m_state_context->current_state()->is_exit_state()) {
        m_state_context->current_state()->perform(cycles);
    }
}

void SynacorApplicationSession::pause()
{
    m_state_context->change_state(m_state_context->paused_state());
}

void SynacorApplicationSession::stop()
{
    m_state_context->change_state(m_state_context->stopped_state());
}

void SynacorApplicationSession::gui_request(GuiRequest request)
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
        m_is_in_debug_mode = request.m_bool_payload;
        break;
    case INPUT_FROM_TERMINAL:
        input_from_terminal(request.m_data_payload);
        break;
    }
}

std::vector<Data> create_work_ram(std::size_t size)
{
    std::vector<Data> work_ram;

    work_ram.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        work_ram.emplace_back(0);
    }

    return work_ram;
}

void SynacorApplicationSession::input_from_terminal(Data input)
{
    m_state_context->m_is_awaiting_input = false;
    m_cpu->input(input);
}

void SynacorApplicationSession::out_changed(Data character)
{
    m_ui->to_terminal(character);
}

void SynacorApplicationSession::in_requested()
{
    m_state_context->m_is_awaiting_input = true;
    m_ui->from_terminal();
}

void SynacorApplicationSession::setup_cpu()
{
    const Address initial_pc(0);

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
    m_cpu->add_in_observer(*this);
}

void SynacorApplicationSession::setup_debugging()
{
    m_debug_container = std::make_shared<DebugContainer<Address, Data, 16>>();
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R0", [&]() { return m_cpu->r0(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R1", [&]() { return m_cpu->r1(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R2", [&]() { return m_cpu->r2(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R3", [&]() { return m_cpu->r3(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R4", [&]() { return m_cpu->r4(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R5", [&]() { return m_cpu->r5(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R6", [&]() { return m_cpu->r6(); }));
    //    m_debug_container->add_register(RegisterDebugContainer<Data>("R7", [&]() { return m_cpu->r7(); }));
    m_debug_container->add_pc([&]() { return m_cpu->pc(); });
//    m_debug_container->add_memory(MemoryDebugContainer<Data>([&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());
    m_debug_container->add_file_content([&]() { return m_file_content; });

    m_ui->attach_debugger(m_debugger);
    m_ui->attach_debug_container(m_debug_container);
    m_ui->attach_logger(m_logger);
}

std::vector<RawData> SynacorApplicationSession::memory()
{
    return { m_memory.begin(), m_memory.end() };
}

std::vector<DisassembledLine<Address, 16>> SynacorApplicationSession::disassemble_program()
{
        std::stringstream ss;
        Disassembler disassembler(m_memory, ss);
        disassembler.disassemble();

        std::vector<std::string> disassembled_program = split(ss, "\n");

        disassembled_program.erase(
            std::remove_if(disassembled_program.begin(), disassembled_program.end(), [](std::string const& s) { return s.empty(); }));

        std::vector<DisassembledLine<Address, 16>> lines;
        std::transform(disassembled_program.begin(), disassembled_program.end(), std::back_inserter(lines),
            [](std::string const& line) { return DisassembledLine<Address, 16>(line); });

        return lines;

//    return {};
}

}
