#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_SESSION_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_SESSION_H

#include <memory>
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "z80/cpu.h"
#include "z80/run_status.h"
#include "z80/applications/pacman/memory_mapped_io.h"
#include "z80/applications/pacman/gui/gui.h"
#include "z80/applications/pacman/interfaces/input.h"
#include "z80/applications/pacman/interfaces/io_observer.h"
#include "z80/interfaces/emulatorZ80.h"
#include "z80/interfaces/gui_observer.h"
#include "z80/interfaces/in_observer.h"
#include "z80/interfaces/out_observer.h"
#include "z80/interfaces/session.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/logging/log_observer.h"
#include "crosscutting/logging/logger.h"

namespace emu::z80::applications::pacman {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::logging::Logger;

    class PacmanSession
            : public Session,
              public GuiObserver,
              public OutObserver,
              public InObserver,
              public IoObserver {
    public:
        PacmanSession(
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input,
                std::shared_ptr<MemoryMappedIo> memory_mapped_io,
                EmulatorMemory &memory
        );

        ~PacmanSession() override;

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
        RunStatus m_run_status;

        u8 m_vblank_interrupt_return;

        std::shared_ptr<MemoryMappedIo> m_memory_mapped_io;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        std::unique_ptr<Cpu> m_cpu;

        EmulatorMemory &m_memory;

        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<Debugger> m_debugger;
        DebugContainer m_debug_container;
        std::unordered_map<u8, u8> m_outputs_during_cycle;

        // Game loop - begin
        static constexpr double fps = 60.0;
        static constexpr long double tick_limit = 1000.0 / fps;
        static constexpr int cycles_per_ms = 2000;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        // Game loop - end

        // IO - begin
        static constexpr int out_port_vblank_interrupt_return = 0;
        // IO - end

        void running(u64 &last_tick, unsigned long &cycles);

        void pausing(u64 &last_tick);

        void stepping(u64 &last_tick, unsigned long &cycles);

        void await_input_and_update_debug();

        void setup_cpu();

        void setup_debugging();

        std::vector<u8> tile_ram();

        std::vector<u8> sprite_ram();

        std::vector<u8> palette_ram();

        std::vector<u8> memory();

        std::vector<std::string> disassemble_program();
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_SESSION_H
