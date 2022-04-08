#include <iostream>
#include <SDL_timer.h>
#include "space_invaders.h"
#include "8080/cpu.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/file_util.h"

namespace emu::cpu8080::applications::space_invaders {

    using emu::cpu8080::RunStatus;
    using emu::util::file::read_file_into_vector;

    SpaceInvaders::SpaceInvaders(
            const Settings &settings,
            std::shared_ptr<Gui> gui,
            std::shared_ptr<Input> input
    )
            : m_is_in_debug_mode(false),
              m_run_status(NOT_RUNNING),
              m_cpu_io(CpuIo(0, 0b00001000, 0)),
              m_gui(std::move(gui)),
              m_input(std::move(input)) {
        load_file();
        setup_cpu();
        setup_debugging();
        m_cpu_io.set_dipswitches(settings);
    }

    void SpaceInvaders::load_file() {
        m_memory.add(read_file_into_vector("invaders.h")); // $0000-$07ff: invaders.h
        m_memory.add(read_file_into_vector("invaders.g")); // $0800-$0fff: invaders.g
        m_memory.add(read_file_into_vector("invaders.f")); // $1000-$17ff: invaders.f
        m_memory.add(read_file_into_vector("invaders.e")); // $1800-$1fff: invaders.e
        m_memory.add(create_work_ram());                   // $2000-$23ff: work RAM
        m_memory.add(create_vram());                       // $2400-$3fff: video RAM
        m_memory.add_link(0x2000, 0x4000);                 // $4000-: RAM mirror
        m_memory.add_link(0x2000, 0x4000);
        m_memory.add_link(0x2000, 0x4000);
        m_memory.add_link(0x2000, 0x4000);
        m_memory.add_link(0x2000, 0x4000);
        m_memory.add_link(0x2000, 0x4000);
    }

    void SpaceInvaders::setup_cpu() {
        const std::uint16_t initial_pc = 0;

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
        m_cpu->add_in_observer(*this);
    }

    void SpaceInvaders::setup_debugging() {
        m_debug_container.add_register("A", [&]() { return m_cpu->a(); });
        m_debug_container.add_register("B", [&]() { return m_cpu->b(); });
        m_debug_container.add_register("C", [&]() { return m_cpu->c(); });
        m_debug_container.add_register("D", [&]() { return m_cpu->d(); });
        m_debug_container.add_register("E", [&]() { return m_cpu->e(); });
        m_debug_container.add_register("H", [&]() { return m_cpu->h(); });
        m_debug_container.add_register("L", [&]() { return m_cpu->l(); });
        m_debug_container.add_pc([&]() { return m_cpu->pc(); });
        m_debug_container.add_sp([&]() { return m_cpu->sp(); });
        m_debug_container.add_is_interrupted([&]() { return m_cpu->is_interrupted(); });
        m_debug_container.add_flag_register("F", [&]() { return m_cpu->f(); },
                                            {
                                                  std::make_tuple("s", 7),
                                                  std::make_tuple("z", 6),
                                                  std::make_tuple("u", 5),
                                                  std::make_tuple("a", 4),
                                                  std::make_tuple("u", 3),
                                                  std::make_tuple("p", 2),
                                                  std::make_tuple("u", 1),
                                                  std::make_tuple("c", 0)
                                          });

        m_gui->attach_debug_container(m_debug_container);
    }

    void SpaceInvaders::run_status_changed(emu::cpu8080::RunStatus new_status) {
        m_run_status = new_status;
    }

    void SpaceInvaders::debug_mode_changed(bool is_in_debug_mode) {
        m_is_in_debug_mode = is_in_debug_mode;
    }

    void SpaceInvaders::in_requested(std::uint8_t port) {
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

    void SpaceInvaders::out_changed(std::uint8_t port) {
        switch (port) {
            case out_port_shift_offset:
                m_cpu_io.m_shift_register.change_offset(m_cpu->a());
                break;
            case out_port_sound_1:
                Audio::play_sound_port_1(m_cpu->a());
                break;
            case out_port_do_shift:
                m_cpu_io.m_shift_register.shift(m_cpu->a());
                break;
            case out_port_sound_2:
                Audio::play_sound_port_2(m_cpu->a());
                break;
            case out_port_watchdog:
                break;
            default:
                throw std::runtime_error("Illegal output port for Space Invaders");
        }
    }

    void SpaceInvaders::io_changed(IoRequest request) {
        switch (request) {
            case IoRequest::BREAK_EXECUTION:
                m_run_status = STEPPING;
                break;
            case IoRequest::CONTINUE_EXECUTION:
                break;
            case IoRequest::STEP_OVER:
                break;
            case IoRequest::STEP_INTO:
                break;
            default:
                throw std::runtime_error("Unhandled IoRequest in io_changed");
        }
    }

    void SpaceInvaders::run() {
        m_gui->add_gui_observer(*this);
        m_input->add_io_observer(*this);
        m_cpu->start();
        m_run_status = RunStatus::RUNNING;

        unsigned long i;
        std::uint64_t last_tick = SDL_GetTicks64();

        while (m_run_status == RUNNING || m_run_status == PAUSED || m_run_status == STEPPING) {
            if (m_run_status == RUNNING) {
                if (SDL_GetTicks64() - last_tick >= tick_limit) {
                    last_tick = SDL_GetTicks();

                    i = 0;
                    while (i < static_cast<long>(cycles_per_tick / 2)) {
                        i += m_cpu->next_instruction();
                    }

                    if (m_cpu->is_inta()) {
                        m_cpu->interrupt(RST_1);
                    }

                    i = 0;
                    while (i < static_cast<long>(cycles_per_tick / 2)) {
                        i += m_cpu->next_instruction();
                    }

                    m_input->read(m_run_status, m_cpu_io);
                    m_gui->update_screen(this->vram(), m_run_status);

                    if (m_cpu->is_inta()) {
                        m_cpu->interrupt(RST_2);
                    }
                }
            } else if (m_run_status == PAUSED) {
                if (SDL_GetTicks64() - last_tick >= tick_limit) {
                    last_tick = SDL_GetTicks();
                    m_input->read(m_run_status, m_cpu_io);
                    m_gui->update_screen(this->vram(), m_run_status);
                }
            } else if (m_run_status == STEPPING) {
                throw std::runtime_error("Stepping has not been implemented yet");
            } else {
                throw std::runtime_error("Some kind of run_status has not been handled in the main loop");
            }
        }
    }

    std::vector<std::uint8_t> SpaceInvaders::create_work_ram() {
        std::vector<std::uint8_t> work_ram;
        const int size = 1024;

        work_ram.reserve(size);
        for (int i = 0; i < size; ++i) {
            work_ram.push_back(0);
        }

        return work_ram;
    }

    std::vector<std::uint8_t> SpaceInvaders::create_vram() {
        std::vector<std::uint8_t> vram;
        const int size = 7168;

        vram.reserve(size);
        for (int i = 0; i < size + 1; ++i) {
            vram.push_back(0);
        }

        return vram;
    }

    std::vector<std::uint8_t> SpaceInvaders::vram() {
        return {m_memory.begin() + 0x2400, m_memory.begin() + 0x3fff};
    }
}
