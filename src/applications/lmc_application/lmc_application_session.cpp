#include "lmc_application_session.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "interfaces/input.h"
#include "lmc/cpu.h"
#include "ui.h"
#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace emu::applications::lmc {

    using emu::debugger::FlagRegisterDebugContainer;
    using emu::debugger::MemoryDebugContainer;
    using emu::debugger::RegisterDebugContainer;
    using emu::lmc::Address;
    using emu::lmc::Data;
    using emu::lmc::OutType;
    using emu::misc::sdl_get_ticks_high_performance;
    using emu::misc::RunStatus::FINISHED;
    using emu::misc::RunStatus::NOT_RUNNING;
    using emu::misc::RunStatus::PAUSED;
    using emu::misc::RunStatus::RUNNING;
    using emu::misc::RunStatus::STEPPING;
    using emu::util::byte::to_u16;

    LmcApplicationSession::LmcApplicationSession(
            const RunStatus startup_runstatus,
            std::shared_ptr<Ui> gui,
            std::shared_ptr<Input> input,
            std::string loaded_file,
            EmulatorMemory<Address, Data> memory
    )
        : m_is_in_debug_mode(false),
          m_is_stepping_instruction(false),
          m_is_stepping_cycle(false),
          m_is_continuing_execution(false),
          m_startup_runstatus(startup_runstatus),
          m_run_status(NOT_RUNNING),
          m_gui(std::move(gui)),
          m_input(std::move(input)),
          m_memory(std::move(memory)),
          m_loaded_file(std::move(loaded_file)),
          m_logger(std::make_shared<Logger>()),
          m_debugger(std::make_shared<Debugger>()),
          m_governor(Governor(tick_limit, sdl_get_ticks_high_performance)) {
        setup_cpu();
        setup_debugging();

        m_gui->add_gui_observer(*this);
    }

    LmcApplicationSession::~LmcApplicationSession() {
        m_gui->remove_gui_observer(this);
    }

    void LmcApplicationSession::run() {
        if (m_run_status == FINISHED) {
            throw std::invalid_argument("Programming error: the session has finished and cannot be run one more time");
        }

        m_cpu->start();
        m_run_status = m_startup_runstatus;

        cyc cycles;

        while (m_run_status == RUNNING || m_run_status == PAUSED || m_run_status == STEPPING) {
            if (m_run_status == RUNNING) {
                running(cycles);
            } else if (m_run_status == PAUSED) {
                pausing();
            } else {
                stepping(cycles);
            }
        }

        m_run_status = FINISHED;
    }

    void LmcApplicationSession::running(cyc &cycles) {
        if (m_governor.is_time_to_update()) {
            cycles = 0;
            while (cycles < static_cast<cyc>(cycles_per_tick)) {
                if (m_cpu->can_run_next_instruction()) {
                    m_cpu->next_instruction();
                } else {
                    m_run_status = FINISHED;
                }
                ++cycles;
                if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc().underlying())) {
                    m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                    m_run_status = STEPPING;
                    return;
                }
            }

            m_input->read(m_run_status);
            m_gui->update_screen(m_run_status);
        }
    }

    void LmcApplicationSession::pausing() {
        if (m_governor.is_time_to_update()) {
            m_input->read(m_run_status);
            m_gui->update_screen(m_run_status);
        }
    }

    void LmcApplicationSession::stepping(cyc &cycles) {
        await_input_and_update_debug();
        if (m_run_status == NOT_RUNNING) {
            return;
        }

        cycles = 0;
        while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
            m_cpu->next_instruction();
            ++cycles;
            if (!m_is_stepping_cycle && !m_is_continuing_execution) {
                await_input_and_update_debug();
            }
            if (m_run_status == NOT_RUNNING) {
                return;
            }
        }

        m_input->read(m_run_status);
        m_gui->update_screen(m_run_status);

        m_is_stepping_cycle = false;
        if (m_is_continuing_execution) {
            m_is_continuing_execution = false;
            m_run_status = RUNNING;
        }
    }

    void LmcApplicationSession::await_input_and_update_debug() {
        while (!m_is_stepping_instruction && !m_is_stepping_cycle && !m_is_continuing_execution) {
            m_input->read_debug_only(m_run_status);
            if (m_run_status == NOT_RUNNING) {
                return;
            }

            m_gui->update_debug_only();
        }

        m_is_stepping_instruction = false;
    }

    void LmcApplicationSession::pause() {
        throw std::runtime_error("Pause is not implemented for LMC programs");
    }

    void LmcApplicationSession::stop() {
        throw std::runtime_error("Stop is not implemented for LMC programs");
    }

    void LmcApplicationSession::run_status_changed(RunStatus new_status) {
        m_run_status = new_status;
    }

    void LmcApplicationSession::debug_mode_changed(bool is_in_debug_mode) {
        m_is_in_debug_mode = is_in_debug_mode;
    }

    void LmcApplicationSession::out_changed(Data acc_reg, OutType out_type) {
        if (out_type == OutType::OUT) {
            std::cout << acc_reg.underlying() << "\n";
        } else {
            std::cout << static_cast<char>(acc_reg.underlying());
        }
    }

    void LmcApplicationSession::in_requested() {
        std::cout << "inp: " << std::flush;

        u16 number;
        std::cin >> number;

        m_cpu->input(Data(number));
    }

    void LmcApplicationSession::setup_cpu() {
        const Address initial_pc(0);

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
        m_cpu->add_in_observer(*this);
    }

    void LmcApplicationSession::setup_debugging() {
        m_debug_container.add_register(RegisterDebugContainer<Data>("A", [&]() { return m_cpu->a(); }));
        m_debug_container.add_pc([&]() { return m_cpu->pc().underlying(); });
//        m_debug_container.add_flag_register(FlagRegisterDebugContainer(
//                "F",
//                [&]() { return m_cpu->m_flag_reg; },
//                {{"s", 7},
//                 {"z", 6},
//                 {"u", 5},
//                 {"a", 4},
//                 {"u", 3},
//                 {"p", 2},
//                 {"u", 1},
//                 {"c", 0}}
//        ));
//        m_debug_container.add_memory(MemoryDebugContainer(
//                [&]() { return memory(); }
//        ));
//        m_debug_container.add_disassembled_program(disassemble_program());

        m_gui->attach_debugger(m_debugger);
        m_gui->attach_debug_container(m_debug_container);
        m_gui->attach_logger(m_logger);
    }
}
