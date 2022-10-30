#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_SESSION_H

#include <memory>
#include <SDL.h>
#include <unordered_map>
#include <vector>
#include "audio.h"
#include "gui.h"
#include "memory_mapped_io_for_pacman.h"
#include "interfaces/input.h"
#include "interfaces/io_observer.h"
#include "chips/z80/cpu.h"
#include "chips/z80/interfaces/gui_observer.h"
#include "chips/z80/interfaces/in_observer.h"
#include "chips/z80/interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/disassembled_line.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/logging/logger.h"

namespace emu::applications::pacman {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::debugger::DisassembledLine;
    using emu::logging::Logger;
    using emu::misc::Governor;
    using emu::misc::Session;
    using emu::z80::Cpu;
    using emu::z80::EmulatorMemory;
    using emu::z80::GuiObserver;
    using emu::z80::InObserver;
    using emu::z80::OutObserver;
    using emu::z80::RunStatus;

    class PacmanSession
            : public Session,
              public GuiObserver,
              public OutObserver,
              public IoObserver {
    public:
        PacmanSession(
                const RunStatus startup_runstatus,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input,
                std::shared_ptr<Audio> audio,
                std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
                EmulatorMemory &memory
        );

        ~PacmanSession() override;

        void run() override;

        void pause() override;

        void stop() override;

        void run_status_changed(RunStatus new_status) override;

        void debug_mode_changed(bool is_in_debug_mode) override;

        void out_changed(u8 port) override;

        void io_changed(IoRequest request) override;

    private:
        static constexpr long double fps = 60.0L;
        static constexpr long double tick_limit = 1000.0L / fps;
        static constexpr int cycles_per_ms = 3072;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        static constexpr int out_port_vblank_interrupt_return = 0;

        bool m_is_in_debug_mode;
        bool m_is_stepping_instruction;
        bool m_is_stepping_cycle;
        bool m_is_continuing_execution;
        RunStatus m_startup_runstatus;
        RunStatus m_run_status;

        u8 m_vblank_interrupt_return;

        std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        std::shared_ptr<Audio> m_audio;
        std::unique_ptr<Cpu> m_cpu;

        EmulatorMemory &m_memory;

        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<Debugger> m_debugger;
        DebugContainer m_debug_container;
        std::unordered_map<u8, u8> m_outputs_during_cycle;

        Governor m_governor;

        void running(cyc &cycles);

        void pausing();

        void stepping(cyc &cycles);

        void await_input_and_update_debug();

        void setup_cpu();

        void setup_debugging();

        std::vector<u8> tile_ram();

        std::vector<u8> sprite_ram();

        std::vector<u8> palette_ram();

        std::vector<u8> memory();

        std::vector<DisassembledLine> disassemble_program();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_SESSION_H
