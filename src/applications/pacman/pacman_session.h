#pragma once

#include "chips/z80/interfaces/gui_observer.h"
#include "chips/z80/interfaces/out_observer.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "gui_io.h"
#include "interfaces/io_observer.h"
#include "pacman/interfaces/state_manager.h"
#include "pacman/io_request.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace emu::applications::pacman {
class Audio;
class Gui;
class Input;
class MemoryMappedIoForPacman;
class State;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class DisassembledLine;
}
namespace emu::logging {
class Logger;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::z80 {
class Cpu;
class InObserver;
}

namespace emu::applications::pacman {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;
using emu::z80::Cpu;
using emu::z80::GuiObserver;
using emu::z80::InObserver;
using emu::z80::OutObserver;
using emu::z80::RunStatus;
using emu::z80::RunStatus::NOT_RUNNING;

class PacmanSession
    : public Session
    , public GuiObserver
    , public OutObserver
    , public IoObserver
    , public StateManager {
public:
    PacmanSession(
        const RunStatus startup_runstatus,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        std::shared_ptr<Audio> audio,
        std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
        EmulatorMemory<u16, u8>& memory);

    ~PacmanSession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void run_status_changed(RunStatus new_status) override;

    void debug_mode_changed(bool is_in_debug_mode) override;

    void out_changed(u8 port) override;

    void io_changed(IoRequest request) override;

    void change_state(std::shared_ptr<State> new_state) override;

    std::shared_ptr<State> paused_state() override;

    std::shared_ptr<State> running_state() override;

    std::shared_ptr<State> stepping_state() override;

    std::shared_ptr<State> stopped_state() override;

    std::shared_ptr<State> current_state() override;

private:
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 3072;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    static constexpr int s_out_port_vblank_interrupt_return = 0;

    bool m_is_in_debug_mode { false };
    RunStatus m_run_status { NOT_RUNNING };

    u8 m_vblank_interrupt_return { 0 };

    GuiIo m_gui_io;
    std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Audio> m_audio;
    std::shared_ptr<Cpu> m_cpu;

    EmulatorMemory<u16, u8>& m_memory;

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<u16, 16>> m_debugger;
    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
    std::unordered_map<u8, u8> m_outputs_during_cycle;

    Governor m_governor { Governor(s_tick_limit, sdl_get_ticks_high_performance) };

    std::shared_ptr<State> m_current_state;
    std::shared_ptr<State> m_running_state;
    std::shared_ptr<State> m_paused_state;
    std::shared_ptr<State> m_stepping_state;
    std::shared_ptr<State> m_stopped_state;

    void setup_cpu();

    void setup_debugging();

    std::vector<u8> memory();

    std::vector<DisassembledLine<u16, 16>> disassemble_program();
};
}
