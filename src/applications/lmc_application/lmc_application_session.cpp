#include "lmc_application_session.h"
#include "chips/trivial/lmc/assembler/assembler.h"
#include "chips/trivial/lmc/cpu.h"
#include "chips/trivial/lmc/disassembler.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace emu::applications::lmc {

using emu::debugger::DisassembledLine;
using emu::debugger::FlagRegisterDebugContainer;
using emu::debugger::MemoryDebugContainer;
using emu::debugger::RegisterDebugContainer;
using emu::lmc::Address;
using emu::lmc::Assembler;
using emu::lmc::Data;
using emu::lmc::Disassembler;
using emu::lmc::OutType;
using emu::util::byte::to_u16;
using emu::util::string::split;

LmcApplicationSession::LmcApplicationSession(
    bool is_only_run_once,
    bool is_starting_paused,
    std::shared_ptr<Ui> gui,
    std::shared_ptr<Input> input,
    std::string loaded_file,
    std::string file_content,
    EmulatorMemory<Address, Data> memory)
    : m_is_only_run_once(is_only_run_once)
    , m_ui(std::move(gui))
    , m_input(std::move(input))
    , m_memory(std::move(memory))
    , m_loaded_file(std::move(loaded_file))
    , m_file_content(std::move(file_content))
    , m_logger(std::make_shared<Logger>())
    , m_debugger(std::make_shared<Debugger<Address, 10>>())
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

LmcApplicationSession::~LmcApplicationSession()
{
    m_ui->remove_ui_observer(this);
    m_cpu->remove_out_observer(this);
    m_cpu->remove_in_observer(this);
}

std::function<void()> loop;
void main_loop() {
    loop();
}

void LmcApplicationSession::run()
{
    m_cpu->start();

    cyc cycles;

#ifdef __EMSCRIPTEN__
    loop = [&] {
        m_state_context->current_state()->perform(cycles);
        if (m_state_context->current_state()->is_exit_state()) {
            std::cout << "Exit\n";
            exit(0);
        }
    };
    emscripten_set_main_loop(main_loop, 60, true);
#else
    while (!m_state_context->current_state()->is_exit_state()) {
        m_state_context->current_state()->perform(cycles);
    }
#endif
}

void LmcApplicationSession::pause()
{
    m_state_context->change_state(m_state_context->paused_state());
}

void LmcApplicationSession::stop()
{
    m_state_context->change_state(m_state_context->stopped_state());
}

void LmcApplicationSession::gui_request(GuiRequest request)
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
    case SOURCE_CODE:
        m_file_content = request.m_string_payload;
        break;
    case ASSEMBLE_AND_LOAD:
        assemble_and_load_request();
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

void LmcApplicationSession::assemble_and_load_request()
{
    m_logger->info("Trying to assemble and load source code...");

    m_cpu->reset_state();
    m_ui->clear_terminal();
    m_state_context->change_state(m_state_context->paused_state());

    try {
        std::stringstream ss(m_file_content);
        const std::vector<Data> code = Assembler::assemble(ss);
        std::vector<Data> remaining_memory;

        if (code.size() < m_memory.size()) {
            remaining_memory = create_work_ram(m_memory.size() - code.size());
        }

        if (code.size() + remaining_memory.size() == m_memory.size()) {
            m_memory.clear();
            m_memory.add(code);
            m_memory.add(remaining_memory);
            m_logger->info("Ok");
        } else {
            m_memory.clear();
            m_memory.add({ code.begin(), code.begin() + s_program_size });
            m_logger->info("The program is too large: truncating it");
        }
    } catch (std::exception& ex) {
        m_logger->info(ex.what());
    }

    m_debug_container->add_disassembled_program(disassemble_program());
}

void LmcApplicationSession::input_from_terminal(Data input)
{
    m_state_context->m_is_awaiting_input = false;
    m_cpu->input(input);
}

void LmcApplicationSession::out_changed(Data acc_reg, OutType out_type)
{
    m_ui->to_terminal(acc_reg, out_type);
}

void LmcApplicationSession::in_requested()
{
    m_state_context->m_is_awaiting_input = true;
    m_ui->from_terminal();
}

void LmcApplicationSession::setup_cpu()
{
    const Address initial_pc(0);

    m_cpu = std::make_shared<Cpu>(m_memory, initial_pc);

    m_cpu->add_out_observer(*this);
    m_cpu->add_in_observer(*this);
}

void LmcApplicationSession::setup_debugging()
{
    m_debug_container = std::make_shared<DebugContainer<Address, Data, 10>>();
    m_debug_container->add_register(RegisterDebugContainer<Data>("A", [&]() { return m_cpu->a(); }));
    m_debug_container->add_pc([&]() { return m_cpu->pc(); });
    m_debug_container->add_flag_register(FlagRegisterDebugContainer<Data>(
        "F",
        [&]() { return Data(m_cpu->f()); },
        { { "n", 0 } }));
    m_debug_container->add_memory(MemoryDebugContainer<Data>([&]() { return memory(); }));
    m_debug_container->add_disassembled_program(disassemble_program());
    m_debug_container->add_file_content([&]() { return m_file_content; });

    m_ui->attach_debugger(m_debugger);
    m_ui->attach_debug_container(m_debug_container);
    m_ui->attach_logger(m_logger);
}

std::vector<Data> LmcApplicationSession::memory()
{
    return { m_memory.begin(), m_memory.end() };
}

std::vector<DisassembledLine<Address, 10>> LmcApplicationSession::disassemble_program()
{
    std::stringstream ss;
    Disassembler disassembler(m_memory, ss);
    disassembler.disassemble();

    std::vector<std::string> disassembled_program = split(ss, "\n");

    disassembled_program.erase(
        std::remove_if(disassembled_program.begin(), disassembled_program.end(), [](std::string const& s) { return s.empty(); }));

    std::vector<DisassembledLine<Address, 10>> lines;
    std::transform(
        disassembled_program.begin(),
        disassembled_program.end(),
        std::back_inserter(lines),
        [](std::string const& line) { return DisassembledLine<Address, 10>(line); });

    return lines;
}

}
