#include <iostream>
#include <utility>
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
            : run_status(NOT_RUNNING),
              cpu_io(Io(0, 0b00001000, 0)),
              gui(std::move(gui)),
              input(std::move(input)) {
        load_file();
        cpu_io.set_dipswitches(settings);
    }

    void SpaceInvaders::load_file() {
        // Memory map:
        memory.add(read_file_into_vector("invaders.h")); // $0000-$07ff: invaders.h
        memory.add(read_file_into_vector("invaders.g")); // $0800-$0fff: invaders.g
        memory.add(read_file_into_vector("invaders.f")); // $1000-$17ff: invaders.f
        memory.add(read_file_into_vector("invaders.e")); // $1800-$1fff: invaders.e
        memory.add(create_work_ram());                   // $2000-$23ff: work RAM
        memory.add(create_vram());                       // $2400-$3fff: video RAM
        memory.add_link(0x2000, 0x4000);                 // $4000-: RAM mirror
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);
        memory.add_link(0x2000, 0x4000);

        const std::uint16_t initial_PC = 0;

        cpu = std::make_unique<Cpu>(memory, initial_PC);

        cpu->add_out_observer(*this);
        cpu->add_in_observer(*this);
    }

    void SpaceInvaders::run_status_changed(emu::cpu8080::RunStatus new_status) {
        run_status = new_status;
    }

    void SpaceInvaders::in_requested(std::uint8_t port) {
        switch (port) {
            case in_port_unused:
                cpu->input(in_port_unused, cpu_io.in_port0);
                break;
            case in_port_1:
                cpu->input(in_port_1, cpu_io.in_port1);
                break;
            case in_port_2:
                cpu->input(in_port_2, cpu_io.in_port2);
                break;
            case in_port_read_shift:
                cpu->input(in_port_read_shift, cpu_io.shift_register.read());
                break;
            default:
                throw std::runtime_error("Illegal input port for Space Invaders");
        }
    }

    void SpaceInvaders::out_changed(std::uint8_t port) {
        switch (port) {
            case out_port_shift_offset:
                cpu_io.shift_register.change_offset(cpu->A());
                break;
            case out_port_sound_1: {
                /*
                 * Port 3: (discrete sounds)
                 *   bit 0=UFO (repeats)        SX0 0.raw
                 *   bit 1=Shot                 SX1 1.raw
                 *   bit 2=Flash (player die)   SX2 2.raw
                 *   bit 3=Invader die          SX3 3.raw
                 *   bit 4=Extended play        SX4
                 *   bit 5= AMP enable          SX5
                 *   bit 6= NC (not wired)
                 *   bit 7= NC (not wired)
                 *   Port 4: (discrete sounds)
                 *   bit 0-7 shift data (LSB on 1st write, MSB on 2nd)
                 */
                if (cpu->A() & (1 << 3)) {  // invader die
                }

                break;
            }
            case out_port_do_shift:
                cpu_io.shift_register.shift(cpu->A());
                break;
            case out_port_sound_2:
                break;
            case out_port_watchdog:
                break;
            default:
                throw std::runtime_error("Illegal output port for Space Invaders");
        }
    }

    void SpaceInvaders::run() {
        gui->add_gui_observer(*this);
        cpu->start();
        run_status = RunStatus::RUNNING;

        unsigned long i;

        std::uint64_t last_tick = SDL_GetTicks64();
        while (run_status == RunStatus::RUNNING || run_status == RunStatus::PAUSED) {
            if (run_status == RunStatus::RUNNING) {
                if (SDL_GetTicks64() - last_tick >= tick_limit) {
                    last_tick = SDL_GetTicks();

                    i = 0;
                    while (i < static_cast<long>(cycles_per_tick / 2)) {
                        i += cpu->next_instruction();
                    }

                    if (cpu->is_inta()) {
                        cpu->interrupt(RST_1);
                    }

                    i = 0;
                    while (i < static_cast<long>(cycles_per_tick / 2)) {
                        i += cpu->next_instruction();
                    }

                    input->read(run_status, cpu_io);
                    gui->update_screen(this->vram(), run_status);

                    if (cpu->is_inta() ) {
                        cpu->interrupt(RST_2);
                    }
                }
            } else {
                if (SDL_GetTicks64() - last_tick >= tick_limit) {
                    last_tick = SDL_GetTicks();
                    input->read(run_status, cpu_io);
                    gui->update_screen(this->vram(), run_status);
                }
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
        return {memory.begin() + 0x2400, memory.begin() + 0x3fff};
    }
}
