#pragma once

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

namespace emu::applications::game_boy {
class Audio;
class Gui;
class Input;
class MemoryMappedIoForGameBoy;
class StateContext;
struct GuiRequest;
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
namespace emu::lr35902 {
class Cpu;
}

namespace emu::applications::game_boy {

using emu::applications::game_boy::GuiObserver;
using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;
using emu::lr35902::Cpu;

class GameBoySession
    : public Session
    , public GuiObserver
    , public KeyObserver {
public:
    GameBoySession(
        bool is_starting_paused,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        std::shared_ptr<Audio> audio,
        std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io,
        EmulatorMemory<u16, u8>& memory);

    ~GameBoySession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void gui_request(GuiRequest request) override;

    void key_pressed(IoRequest request) override;

private:
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 3072;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    static constexpr int s_out_port_vblank_interrupt_return = 0;

    bool m_is_in_debug_mode { false };

    u8 m_vblank_interrupt_return { 0 };

    GuiIo m_gui_io;
    std::shared_ptr<MemoryMappedIoForGameBoy> m_memory_mapped_io;
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

    std::shared_ptr<StateContext> m_state_context;

    void setup_cpu();

    void setup_debugging();

    std::vector<u8> memory();

    std::vector<DisassembledLine<u16, 16>> disassemble_program();
};
}
