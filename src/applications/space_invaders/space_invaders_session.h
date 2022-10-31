#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H

#include "audio.h"
#include "chips/8080/cpu.h"
#include "chips/8080/interfaces/gui_observer.h"
#include "chips/8080/interfaces/in_observer.h"
#include "chips/8080/interfaces/out_observer.h"
#include "cpu_io.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include "interfaces/input.h"
#include "interfaces/io_observer.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace emu::applications::space_invaders {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::debugger::DisassembledLine;
    using emu::i8080::Cpu;
    using emu::i8080::EmulatorMemory;
    using emu::i8080::GuiObserver;
    using emu::i8080::InObserver;
    using emu::i8080::OutObserver;
    using emu::logging::Logger;
    using emu::misc::Governor;
    using emu::misc::RunStatus;
    using emu::misc::Session;

    class SpaceInvadersSession
        : public Session,
          public GuiObserver,
          public OutObserver,
          public InObserver,
          public IoObserver {
    public:
        SpaceInvadersSession(
                const Settings &settings,
                const RunStatus startup_runstatus,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input,
                EmulatorMemory &memory
        );

        ~SpaceInvadersSession() override;

        void run() override;

        void pause() override;

        void stop() override;

        void run_status_changed(RunStatus new_status) override;

        void debug_mode_changed(bool is_in_debug_mode) override;

        void in_requested(u8 port) override;

        void out_changed(u8 port) override;

        void io_changed(IoRequest request) override;

    private:
        bool m_is_in_debug_mode;
        bool m_is_stepping_instruction;
        bool m_is_stepping_cycle;
        bool m_is_continuing_execution;
        RunStatus m_startup_runstatus;
        RunStatus m_run_status;

        CpuIo m_cpu_io;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        std::unique_ptr<Cpu> m_cpu;
        Audio m_audio;

        EmulatorMemory &m_memory;

        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<Debugger> m_debugger;
        DebugContainer m_debug_container;
        std::unordered_map<u8, u8> m_outputs_during_cycle;

        Governor m_governor;

        // Game loop - begin
        static constexpr long double fps = 60.0L;
        static constexpr long double tick_limit = 1000.0L / fps;
        static constexpr int cycles_per_ms = 2000;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        // Game loop - end

        // IO - begin
        static constexpr u8 in_port_unused = 0;
        static constexpr u8 in_port_1 = 1;
        static constexpr u8 in_port_2 = 2;
        static constexpr u8 in_port_read_shift = 3;

        static constexpr u8 out_port_shift_offset = 2;
        static constexpr u8 out_port_sound_1 = 3;
        static constexpr u8 out_port_do_shift = 4;
        static constexpr u8 out_port_sound_2 = 5;
        static constexpr u8 out_port_watchdog = 6;
        // IO - end

        static constexpr unsigned int rst_1_i8080 = 0xcf;
        static constexpr unsigned int rst_2_i8080 = 0xd7;

        void running(cyc &cycles);

        void pausing();

        void stepping(cyc &cycles);

        void await_input_and_update_debug();

        void setup_cpu();

        void setup_debugging();

        std::vector<u8> vram();

        std::vector<u8> memory();

        std::vector<DisassembledLine> disassemble_program();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_SESSION_H
