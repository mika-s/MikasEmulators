#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H

#include <cstdint>
#include <memory>
#include <vector>
#include "8080/cpu.h"
#include "8080/debug_container.h"
#include "8080/run_status.h"
#include "8080/shift_register.h"
#include "8080/applications/space_invaders/audio.h"
#include "8080/applications/space_invaders/cpu_io.h"
#include "8080/applications/space_invaders/gui.h"
#include "8080/applications/space_invaders/interfaces/input.h"
#include "8080/applications/space_invaders/interfaces/io_observer.h"
#include "8080/instructions/instructions.h"
#include "8080/interfaces/emulator8080.h"
#include "8080/interfaces/gui_observer.h"
#include "8080/interfaces/in_observer.h"
#include "8080/interfaces/out_observer.h"
#include "8080/interfaces/session.h"

namespace emu::cpu8080::applications::space_invaders {

    class SpaceInvadersSession : public Session, public GuiObserver, public OutObserver, public InObserver, public IoObserver {
    public:
        SpaceInvadersSession(
                const Settings &settings,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input,
                EmulatorMemory memory
        );

        ~SpaceInvadersSession() override;

        void run() override;

        void pause() override;

        void stop() override;

        void run_status_changed(emu::cpu8080::RunStatus new_status) override;

        void debug_mode_changed(bool is_in_debug_mode) override;

        void in_requested(std::uint8_t port) override;

        void out_changed(std::uint8_t port) override;

        void io_changed(IoRequest request) override;

    private:
        bool m_is_in_debug_mode;
        emu::cpu8080::RunStatus m_run_status;

        CpuIo m_cpu_io;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        std::unique_ptr<Cpu> m_cpu;

        EmulatorMemory m_memory;

        DebugContainer m_debug_container;

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

        void setup_cpu();

        void setup_debugging();

        std::vector<std::uint8_t> vram();

        std::vector<std::string> disassemble_program();
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H
