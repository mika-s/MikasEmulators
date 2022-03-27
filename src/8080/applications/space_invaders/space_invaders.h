#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H

#include <cstdint>
#include <vector>
#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>
#include "8080/cpu.h"
#include "8080/debug_container.h"
#include "8080/run_status.h"
#include "8080/shift_register.h"
#include "8080/applications/space_invaders/interfaces/input.h"
#include "8080/applications/space_invaders/io.h"
#include "8080/applications/space_invaders/interfaces/gui.h"
#include "8080/instructions/instructions.h"
#include "8080/interfaces/emulator8080.h"
#include "8080/interfaces/gui_observer.h"
#include "8080/interfaces/in_observer.h"
#include "8080/interfaces/out_observer.h"

namespace emu::cpu8080::applications::space_invaders {

    class SpaceInvaders : public Emulator8080, public GuiObserver, public OutObserver, public InObserver {
    public:
        SpaceInvaders(
                const Settings &settings,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input
        );

        void run() override;

        void run_status_changed(emu::cpu8080::RunStatus new_status) override;

        void in_requested(std::uint8_t port) override;

        void out_changed(std::uint8_t port) override;

    private:
        emu::cpu8080::RunStatus run_status;

        Io cpu_io;
        std::shared_ptr<Gui> gui;
        std::shared_ptr<Input> input;
        std::unique_ptr<Cpu> cpu;

        EmulatorMemory memory;

        DebugContainer debug_container;

        // Game loop - begin
        static constexpr double fps = 60.0;
        static constexpr long double tick_limit = 1000.0 / fps;
        static constexpr int cycles_per_ms = 2000;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        // Game loop - end

        // IO - begin
        static constexpr int in_port_unused = 0;
        static constexpr int in_port_1 = 1;
        static constexpr int in_port_2 = 2;
        static constexpr int in_port_read_shift = 3;

        static constexpr int out_port_shift_offset = 2;
        static constexpr int out_port_sound_1 = 3;
        static constexpr int out_port_do_shift = 4;
        static constexpr int out_port_sound_2 = 5;
        static constexpr int out_port_watchdog = 6;
        // IO - end

        void load_file();

        void setup_cpu();

        void setup_debugging();

        std::vector<std::uint8_t> vram();

        static std::vector<uint8_t> create_work_ram();

        static std::vector<uint8_t> create_vram();
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
