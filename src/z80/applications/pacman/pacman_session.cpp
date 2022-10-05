#include <iostream>
#include <memory>
#include <utility>
#include <SDL_timer.h>
#include "pacman_session.h"
#include "z80/disassemblerZ80.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80::applications::pacman {

    using emu::debugger::FlagRegisterDebugContainer;
    using emu::debugger::IoDebugContainer;
    using emu::debugger::MemoryDebugContainer;
    using emu::debugger::RegisterDebugContainer;
    using emu::util::string::split;

    PacmanSession::PacmanSession(
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input,
            std::shared_ptr<MemoryMappedIo> memory_mapped_io,
            EmulatorMemory &memory
    )
            : m_is_in_debug_mode(false),
              m_is_stepping_instruction(false),
              m_is_stepping_cycle(false),
              m_is_continuing_execution(false),
              m_run_status(NOT_RUNNING),
              m_vblank_interrupt_return(0),
              m_memory_mapped_io(std::move(memory_mapped_io)),
              m_gui(std::move(gui)),
              m_input(std::move(input)),
              m_memory(memory),
              m_logger(std::make_shared<Logger>()),
              m_debugger(std::make_shared<Debugger>()) {
        setup_cpu();
        setup_debugging();

        m_gui->add_gui_observer(*this);
        m_input->add_io_observer(*this);
    }

    PacmanSession::~PacmanSession() {
        m_gui->remove_gui_observer(this);
        m_input->remove_io_observer(this);
        m_cpu->remove_in_observer(this);
        m_cpu->remove_out_observer(this);
    }

    void PacmanSession::run() {
        if (m_run_status == FINISHED) {
            throw std::invalid_argument("Programming error: the session has finished and cannot be run one more time");
        }

        m_cpu->start();
        m_run_status = RUNNING;

        unsigned long cycles;
        u64 last_tick = SDL_GetTicks64();

        while (m_run_status == RUNNING || m_run_status == PAUSED || m_run_status == STEPPING) {
            if (m_run_status == RUNNING) {
                running(last_tick, cycles);
            } else if (m_run_status == PAUSED) {
                pausing(last_tick);
            } else {
                stepping(last_tick, cycles);
            }
        }

        m_run_status = FINISHED;
    }

    void PacmanSession::running(u64 &last_tick, unsigned long &cycles) {
        m_outputs_during_cycle.clear();

        if (SDL_GetTicks64() - last_tick >= tick_limit) {
            last_tick = SDL_GetTicks();

            cycles = 0;
            while (cycles < static_cast<long>(cycles_per_tick)) {
                cycles += m_cpu->next_instruction();
                if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                    m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                    m_run_status = STEPPING;
                    return;
                }
            }

            m_input->read(m_run_status, m_memory_mapped_io);
            m_gui->update_screen(
                    tile_ram(),
                    sprite_ram(),
                    palette_ram(),
                    m_run_status
            );

            if (m_memory_mapped_io->is_interrupt_enabled()) {
                m_cpu->interrupt(m_vblank_interrupt_return);
            }

            m_memory_mapped_io->set_initial_values();
        }
    }

    void PacmanSession::pausing(u64 &last_tick) {
        if (SDL_GetTicks64() - last_tick >= tick_limit) {
            last_tick = SDL_GetTicks();
            m_input->read(m_run_status, m_memory_mapped_io);
            m_gui->update_screen(
                    tile_ram(),
                    sprite_ram(),
                    palette_ram(),
                    m_run_status
            );
        }
    }

    void PacmanSession::stepping([[maybe_unused]] u64 &last_tick, unsigned long &cycles) {
        m_outputs_during_cycle.clear();

        await_input_and_update_debug();
        if (m_run_status == NOT_RUNNING) {
            return;
        }

        cycles = 0;
        while (cycles < static_cast<long>(cycles_per_tick)) {
            cycles += m_cpu->next_instruction();
            if (!m_is_stepping_cycle && !m_is_continuing_execution) {
                await_input_and_update_debug();
            }
            if (m_run_status == NOT_RUNNING) {
                return;
            }
        }

        m_input->read(m_run_status, m_memory_mapped_io);
        m_gui->update_screen(
                tile_ram(),
                sprite_ram(),
                palette_ram(),
                m_run_status
        );

        if (m_memory_mapped_io->is_interrupt_enabled()) {
            m_cpu->interrupt(m_vblank_interrupt_return);
        }

        m_is_stepping_cycle = false;
        if (m_is_continuing_execution) {
            m_is_continuing_execution = false;
            m_run_status = RUNNING;
        }
    }

    void PacmanSession::await_input_and_update_debug() {
        while (!m_is_stepping_instruction && !m_is_stepping_cycle && !m_is_continuing_execution) {
            m_input->read_debug_only(m_run_status);
            if (m_run_status == NOT_RUNNING) {
                return;
            }

            m_gui->update_debug_only();
        }

        m_is_stepping_instruction = false;
    }

    void PacmanSession::pause() {
        m_run_status = PAUSED;
    }

    void PacmanSession::stop() {
        m_run_status = FINISHED;
    }

    void PacmanSession::setup_cpu() {
        const u16 initial_pc = 0;

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
        m_cpu->add_in_observer(*this);
    }

    void PacmanSession::setup_debugging() {
        m_debug_container.add_register(RegisterDebugContainer("A", [&]() { return m_cpu->a(); }));
        m_debug_container.add_register(RegisterDebugContainer("B", [&]() { return m_cpu->b(); }));
        m_debug_container.add_register(RegisterDebugContainer("C", [&]() { return m_cpu->c(); }));
        m_debug_container.add_register(RegisterDebugContainer("D", [&]() { return m_cpu->d(); }));
        m_debug_container.add_register(RegisterDebugContainer("E", [&]() { return m_cpu->e(); }));
        m_debug_container.add_register(RegisterDebugContainer("H", [&]() { return m_cpu->h(); }));
        m_debug_container.add_register(RegisterDebugContainer("L", [&]() { return m_cpu->l(); }));
        m_debug_container.add_register(RegisterDebugContainer("I", [&]() { return m_cpu->i(); }));
        m_debug_container.add_register(RegisterDebugContainer("R", [&]() { return m_cpu->r(); }));
        m_debug_container.add_pc([&]() { return m_cpu->pc(); });
        m_debug_container.add_sp([&]() { return m_cpu->sp(); });
        m_debug_container.add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
        m_debug_container.add_interrupt_mode([&]() {
            switch (m_cpu->interrupt_mode()) {
                case InterruptMode::ZERO:
                    return "0";
                case InterruptMode::ONE:
                    return "1";
                case InterruptMode::TWO:
                    return "2";
                default:
                    return "Unknown";
            }
        });
        m_debug_container.add_flag_register(FlagRegisterDebugContainer(
                "F",
                [&]() { return m_cpu->f(); },
                {
                        {"s", 7},
                        {"z", 6},
                        {"y", 5},
                        {"h", 4},
                        {"x", 3},
                        {"p", 2},
                        {"n", 1},
                        {"c", 0}
                }));
        m_debug_container.add_io(IoDebugContainer(
                "vblank return",
                [&]() { return m_outputs_during_cycle.contains(out_port_vblank_interrupt_return); },
                [&]() { return m_outputs_during_cycle[out_port_vblank_interrupt_return]; }
        ));
        m_debug_container.add_memory(MemoryDebugContainer(
                [&]() { return memory(); }
        ));
        m_debug_container.add_disassembled_program(disassemble_program());

        m_gui->attach_debugger(m_debugger);
        m_gui->attach_debug_container(m_debug_container);
        m_gui->attach_logger(m_logger);
    }

    void PacmanSession::run_status_changed(RunStatus new_status) {
        m_run_status = new_status;
    }

    void PacmanSession::debug_mode_changed(bool is_in_debug_mode) {
        m_is_in_debug_mode = is_in_debug_mode;
    }

    void PacmanSession::in_requested(u8 port) {
        switch (port) {
            default:
                throw std::runtime_error("Illegal input port for Pacman");
        }
    }

    void PacmanSession::out_changed(u8 port) {
        if (!m_outputs_during_cycle.contains(port)) {
            m_outputs_during_cycle[port] = m_cpu->a();
        } else {
            m_outputs_during_cycle[port] |= m_cpu->a();
        }

        switch (port) {
            case out_port_vblank_interrupt_return:
                m_vblank_interrupt_return = m_cpu->a();
                break;
            default:
                throw std::runtime_error("Illegal output port for Pacman");
        }
    }

    void PacmanSession::io_changed(IoRequest request) {
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
//                m_audio.toggle_mute();
                break;
            default:
                throw std::runtime_error("Unhandled IoRequest in io_changed");
        }
    }

    std::vector<u8> PacmanSession::tile_ram() {
        return {m_memory.begin() + 0x4000, m_memory.begin() + 0x43ff + 1};
    }

    std::vector<u8> PacmanSession::palette_ram() {
        return {m_memory.begin() + 0x4400, m_memory.begin() + 0x47ff + 1};
    }

    std::vector<u8> PacmanSession::sprite_ram() {
        return {m_memory.begin() + 0x4ff0, m_memory.begin() + 0x506f + 1};
    }

    std::vector<u8> PacmanSession::memory() {
        return {m_memory.begin(), m_memory.begin() + 0x50ff + 1};
    }

    std::vector<DisassembledLine> PacmanSession::disassemble_program() {
        EmulatorMemory sliced_for_disassembly = m_memory.slice(0, 0x3fff);

        std::stringstream ss;
        DisassemblerZ80 disassembler(sliced_for_disassembly, ss);
        disassembler.disassemble();

        std::vector<std::string> disassembled_program = split(ss, "\n");

        std::vector<DisassembledLine> lines;
        std::transform(disassembled_program.begin(), disassembled_program.end(), std::back_inserter(lines),
                       [](const std::string &line) { return DisassembledLine(line); });

        return lines;
    }
}
