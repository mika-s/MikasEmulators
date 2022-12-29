#pragma once

#include "chips/z80/interfaces/in_observer.h"
#include "chips/z80/interfaces/out_observer.h"
#include "cpu_io.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "gui_io.h"
#include "interfaces/gui_observer.h"
#include "interfaces/key_observer.h"
#include "key_request.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class Gui;
class Input;
class Settings;
class StateContext;
struct GuiRequest;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
template<class A, std::size_t B>
class DisassembledLine;
}
namespace emu::z80 {
class Cpu;
}
namespace emu::logging {
class Logger;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::applications::zxspectrum_48k {

using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;
using emu::z80::Cpu;
using emu::z80::InObserver;
using emu::z80::OutObserver;

class ZxSpectrum48kSession
    : public Session
    , public GuiObserver
    , public OutObserver
    , public InObserver
    , public KeyObserver {
public:
    ZxSpectrum48kSession(
        Settings const& settings,
        bool is_starting_paused,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        EmulatorMemory<u16, u8>& memory);

    ~ZxSpectrum48kSession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void gui_request(GuiRequest request) override;

    void in_requested(u16 port) override;

    void out_changed(u16 port) override;

    void key_pressed(KeyRequest request) override;

private:
    // Game loop - begin
    static constexpr long double s_fps = 50.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    // Game loop - end

    // IO - begin
    static constexpr u8 s_port_0xfe = 0xfe;
    static constexpr unsigned int s_mic_bit = 3;
    static constexpr unsigned int s_beep_bit = 4;
    // IO - end

    bool m_is_in_debug_mode { false };

    CpuIo m_cpu_io;
    GuiIo m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Cpu> m_cpu;

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
