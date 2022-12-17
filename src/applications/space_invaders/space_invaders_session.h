#pragma once

#include "audio.h"
#include "chips/8080/interfaces/gui_observer.h"
#include "chips/8080/interfaces/in_observer.h"
#include "chips/8080/interfaces/out_observer.h"
#include "cpu_io.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "gui_io.h"
#include "interfaces/io_observer.h"
#include "io_request.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace emu::applications::space_invaders {
class Gui;
class Input;
class Settings;
class StateContext;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
template<class A, std::size_t B>
class DisassembledLine;
}
namespace emu::i8080 {
class Cpu;
}
namespace emu::logging {
class Logger;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::applications::space_invaders {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::i8080::Cpu;
using emu::i8080::GuiObserver;
using emu::i8080::InObserver;
using emu::i8080::OutObserver;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::RunStatus;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;

class SpaceInvadersSession
    : public Session
    , public GuiObserver
    , public OutObserver
    , public InObserver
    , public IoObserver {
public:
    SpaceInvadersSession(
        Settings const& settings,
        const RunStatus startup_runstatus,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        EmulatorMemory<u16, u8>& memory);

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
    // Game loop - begin
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    // Game loop - end

    // IO - begin
    static constexpr u8 s_in_port_unused = 0;
    static constexpr u8 s_in_port_1 = 1;
    static constexpr u8 s_in_port_2 = 2;
    static constexpr u8 s_in_port_read_shift = 3;

    static constexpr u8 s_out_port_shift_offset = 2;
    static constexpr u8 s_out_port_sound_1 = 3;
    static constexpr u8 s_out_port_do_shift = 4;
    static constexpr u8 s_out_port_sound_2 = 5;
    static constexpr u8 s_out_port_watchdog = 6;
    // IO - end

    bool m_is_in_debug_mode { false };
    RunStatus m_run_status { RunStatus::NOT_RUNNING };

    CpuIo m_cpu_io { CpuIo(0, 0b00001000, 0) };
    GuiIo m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Cpu> m_cpu;
    Audio m_audio;

    EmulatorMemory<u16, u8>& m_memory;

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<u16, 16>> m_debugger;
    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
    std::unordered_map<u8, u8> m_outputs_during_cycle;

    Governor m_governor { Governor(s_tick_limit, sdl_get_ticks_high_performance) };

    std::shared_ptr<StateContext> m_state_context;

    void setup_cpu();

    void setup_debugging();

    std::vector<u8> memory();

    std::vector<DisassembledLine<u16, 16>> disassemble_program();
};
}
