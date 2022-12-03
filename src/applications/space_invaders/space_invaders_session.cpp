#include "space_invaders_session.h"
#include "8080/cpu.h"
#include "8080/shift_register.h"
#include "chips/8080/disassembler.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/util/string_util.h"
#include "space_invaders/audio.h"
#include "space_invaders/cpu_io.h"
#include "space_invaders/gui.h"
#include "space_invaders/interfaces/input.h"
#include <algorithm>
#include <functional>
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
    using emu::misc::sdl_get_ticks_high_performance;
    using emu::misc::RunStatus::FINISHED;
    using emu::misc::RunStatus::NOT_RUNNING;
    using emu::misc::RunStatus::PAUSED;
    using emu::misc::RunStatus::RUNNING;
    using emu::misc::RunStatus::STEPPING;
    using emu::util::string::split;

    SpaceInvadersSession::SpaceInvadersSession(
            const Settings &settings,
            const RunStatus startup_runstatus,
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input,
            EmulatorMemory<u16, u8> &memory
    )
        : m_is_in_debug_mode(false),
          m_is_stepping_instruction(false),
          m_is_stepping_cycle(false),
          m_is_continuing_execution(false),
          m_startup_runstatus(startup_runstatus),
          m_run_status(NOT_RUNNING),
          m_cpu_io(CpuIo(0, 0b00001000, 0)),
          m_gui(std::move(gui)),
          m_input(std::move(input)),
          m_memory(memory),
          m_logger(std::make_shared<Logger>()),
          m_debugger(std::make_shared<Debugger>()),
          m_governor(Governor(tick_limit, sdl_get_ticks_high_performance)) {
        setup_cpu();
        setup_debugging();
        m_cpu_io.set_dipswitches(settings);

        m_gui->add_gui_observer(*this);
        m_input->add_io_observer(*this);
    }

    SpaceInvadersSession::~SpaceInvadersSession() {
        m_gui->remove_gui_observer(this);
        m_input->remove_io_observer(this);
        m_cpu->remove_in_observer(this);
        m_cpu->remove_out_observer(this);
    }

    void SpaceInvadersSession::run() {
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

    void SpaceInvadersSession::running(cyc &cycles) {
        m_outputs_during_cycle.clear();

        if (m_governor.is_time_to_update()) {
            cycles = 0;
            while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
                cycles += m_cpu->next_instruction();
                if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                    m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                    m_run_status = STEPPING;
                    return;
                }
            }

            if (m_cpu->is_inta()) {
                m_cpu->interrupt(rst_1_i8080);
            }

            cycles = 0;
            while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
                cycles += m_cpu->next_instruction();
                if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                    m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                    m_run_status = STEPPING;
                    return;
                }
            }

            m_input->read(m_run_status, m_cpu_io);
            m_gui->update_screen(this->vram(), m_run_status);

            if (m_cpu->is_inta()) {
                m_cpu->interrupt(rst_2_i8080);
            }
        }
    }

    void SpaceInvadersSession::pausing() {
        if (m_governor.is_time_to_update()) {
            m_input->read(m_run_status, m_cpu_io);
            m_gui->update_screen(this->vram(), m_run_status);
        }
    }

    void SpaceInvadersSession::stepping(cyc &cycles) {
        m_outputs_during_cycle.clear();

        await_input_and_update_debug();
        if (m_run_status == NOT_RUNNING) {
            return;
        }

        cycles = 0;
        while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
            cycles += m_cpu->next_instruction();
            if (!m_is_stepping_cycle && !m_is_continuing_execution) {
                await_input_and_update_debug();
            }
            if (m_run_status == NOT_RUNNING) {
                return;
            }
        }

        if (m_cpu->is_inta()) {
            m_cpu->interrupt(rst_1_i8080);
        }

        cycles = 0;
        while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
            cycles += m_cpu->next_instruction();
            if (!m_is_stepping_cycle) {
                await_input_and_update_debug();
            }
            if (m_run_status == NOT_RUNNING) {
                return;
            }
        }

        m_input->read(m_run_status, m_cpu_io);
        m_gui->update_screen(this->vram(), m_run_status);

        if (m_cpu->is_inta()) {
            m_cpu->interrupt(rst_2_i8080);
        }

        m_is_stepping_cycle = false;
        if (m_is_continuing_execution) {
            m_is_continuing_execution = false;
            m_run_status = RUNNING;
        }
    }

    void SpaceInvadersSession::await_input_and_update_debug() {
        while (!m_is_stepping_instruction && !m_is_stepping_cycle && !m_is_continuing_execution) {
            m_input->read_debug_only(m_run_status);
            if (m_run_status == NOT_RUNNING) {
                return;
            }

            m_gui->update_debug_only();
        }

        m_is_stepping_instruction = false;
    }

    void SpaceInvadersSession::pause() {
        m_run_status = PAUSED;
    }

    void SpaceInvadersSession::stop() {
        m_run_status = FINISHED;
    }

    void SpaceInvadersSession::setup_cpu() {
        const u16 initial_pc = 0;

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
        m_cpu->add_in_observer(*this);
    }

    void SpaceInvadersSession::setup_debugging() {
        m_debug_container.add_register(RegisterDebugContainer<u8>("A", [&]() { return m_cpu->a(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("B", [&]() { return m_cpu->b(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("C", [&]() { return m_cpu->c(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("D", [&]() { return m_cpu->d(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("E", [&]() { return m_cpu->e(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("H", [&]() { return m_cpu->h(); }));
        m_debug_container.add_register(RegisterDebugContainer<u8>("L", [&]() { return m_cpu->l(); }));
        m_debug_container.add_pc([&]() { return m_cpu->pc(); });
        m_debug_container.add_sp([&]() { return m_cpu->sp(); });
        m_debug_container.add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
        m_debug_container.add_flag_register(FlagRegisterDebugContainer<u8>(
                "F",
                [&]() { return m_cpu->f(); },
                {{"s", 7},
                 {"z", 6},
                 {"u", 5},
                 {"a", 4},
                 {"u", 3},
                 {"p", 2},
                 {"u", 1},
                 {"c", 0}}
        ));
        m_debug_container.add_io(IoDebugContainer<u8>(
                "shift (change offset)",
                [&]() { return m_outputs_during_cycle.contains(out_port_shift_offset); },
                [&]() { return m_outputs_during_cycle[out_port_shift_offset]; }
        ));
        m_debug_container.add_io(IoDebugContainer<u8>(
                "shift (do shift)",
                [&]() { return m_outputs_during_cycle.contains(out_port_do_shift); },
                [&]() { return m_outputs_during_cycle[out_port_do_shift]; }
        ));
        m_debug_container.add_io(IoDebugContainer<u8>(
                "watchdog",
                [&]() { return m_outputs_during_cycle.contains(out_port_watchdog); },
                [&]() { return m_outputs_during_cycle[out_port_watchdog]; }
        ));
        m_debug_container.add_io(IoDebugContainer<u8>(
                "out sound 1",
                [&]() { return m_outputs_during_cycle.contains(out_port_sound_1); },
                [&]() { return m_outputs_during_cycle[out_port_sound_1]; },
                {{"ufo", 0},
                 {"shot", 1},
                 {"flash", 2},
                 {"invader_die", 3},
                 {"extended_play", 4}}
        ));
        m_debug_container.add_io(IoDebugContainer<u8>(
                "out sound 2",
                [&]() { return m_outputs_during_cycle.contains(out_port_sound_2); },
                [&]() { return m_outputs_during_cycle[out_port_sound_2]; },
                {{"fleet_movement_1", 0},
                 {"fleet_movement_2", 1},
                 {"fleet_movement_3", 2},
                 {"fleet_movement_4", 3},
                 {"ufo_hit", 4}}
        ));
        m_debug_container.add_memory(MemoryDebugContainer<u8>(
                [&]() { return memory(); }
        ));
        m_debug_container.add_disassembled_program(disassemble_program());

        m_gui->attach_debugger(m_debugger);
        m_gui->attach_debug_container(m_debug_container);
        m_gui->attach_logger(m_logger);
    }

    void SpaceInvadersSession::run_status_changed(emu::misc::RunStatus new_status) {
        m_run_status = new_status;
    }

    void SpaceInvadersSession::debug_mode_changed(bool is_in_debug_mode) {
        m_is_in_debug_mode = is_in_debug_mode;
    }

    void SpaceInvadersSession::in_requested(u8 port) {
        switch (port) {
            case in_port_unused:
                m_cpu->input(in_port_unused, m_cpu_io.m_in_port0);
                break;
            case in_port_1:
                m_cpu->input(in_port_1, m_cpu_io.m_in_port1);
                break;
            case in_port_2:
                m_cpu->input(in_port_2, m_cpu_io.m_in_port2);
                break;
            case in_port_read_shift:
                m_cpu->input(in_port_read_shift, m_cpu_io.m_shift_register.read());
                break;
            default:
                throw std::runtime_error("Illegal input port for Space Invaders");
        }
    }

    void SpaceInvadersSession::out_changed(u8 port) {
        if (!m_outputs_during_cycle.contains(port)) {
            m_outputs_during_cycle[port] = m_cpu->a();
        } else {
            m_outputs_during_cycle[port] |= m_cpu->a();
        }

        switch (port) {
            case out_port_shift_offset:
                m_cpu_io.m_shift_register.change_offset(m_cpu->a());
                break;
            case out_port_sound_1:
                m_audio.play_sound_port_1(m_cpu->a());
                break;
            case out_port_do_shift:
                m_cpu_io.m_shift_register.shift(m_cpu->a());
                break;
            case out_port_sound_2:
                m_audio.play_sound_port_2(m_cpu->a());
                break;
            case out_port_watchdog:
                break;
            default:
                throw std::runtime_error("Illegal output port for Space Invaders");
        }
    }

    void SpaceInvadersSession::io_changed(IoRequest request) {
        switch (request) {
            case STEP_INSTRUCTION:
                if (m_is_in_debug_mode) {
                    m_is_stepping_instruction = true;
                }
                break;
            case STEP_CYCLE:
                if (m_is_in_debug_mode) {
                    m_is_stepping_cycle = true;
                }
                break;
            case CONTINUE_EXECUTION:
                if (m_is_in_debug_mode) {
                    m_is_continuing_execution = true;
                }
                break;
            case TOGGLE_MUTE:
                m_audio.toggle_mute();
                break;
        }
    }

    std::vector<u8> SpaceInvadersSession::vram() {
        return {m_memory.begin() + 0x2400, m_memory.begin() + 0x3fff + 1};
    }

    std::vector<u8> SpaceInvadersSession::memory() {
        return {m_memory.begin(), m_memory.begin() + 0x3fff + 1};
    }

    std::vector<DisassembledLine> SpaceInvadersSession::disassemble_program() {
        EmulatorMemory<u16, u8> sliced_for_disassembly = m_memory.slice(0, 0x2000);

        std::stringstream ss;
        Disassembler disassembler(sliced_for_disassembly, ss);
        disassembler.disassemble();

        std::vector<std::string> disassembled_program = split(ss, "\n");

        disassembled_program.erase(
                std::remove_if(disassembled_program.begin(), disassembled_program.end(), [](const std::string &s) { return s.empty(); })
        );

        std::vector<DisassembledLine> lines;
        std::transform(
                disassembled_program.begin(),
                disassembled_program.end(),
                std::back_inserter(lines),
                [](const std::string &line) { return DisassembledLine(line); }
        );

        return lines;
    }
}
