#pragma once

#include "applications/game_boy/gui.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <memory>
#include <unordered_map>

namespace emu::applications::game_boy {
class Audio;
class GuiIo;
class Input;
class MemoryMappedIoForPacman;
class State;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
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
namespace emu::misc {
class Governor;
}

namespace emu::applications::game_boy {

using emu::z80::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;

class StateContext {
public:
    explicit StateContext(
        GuiIo& gui_io,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        std::shared_ptr<Audio> audio,
        std::shared_ptr<Cpu> cpu,
        EmulatorMemory<u16, u8>& memory,
        std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
        u8& vblank_interrupt_return,
        std::shared_ptr<Logger> logger,
        std::shared_ptr<Debugger<u16, 16>> debugger,
        std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container,
        std::unordered_map<u8, u8>& outputs_during_cycle,
        Governor& governor,
        bool& is_in_debug_mode);

    bool& m_is_in_debug_mode;

    GuiIo& m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Audio> m_audio;
    std::shared_ptr<Cpu> m_cpu;

    EmulatorMemory<u16, u8>& m_memory;
    std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;

    u8& m_vblank_interrupt_return;

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<u16, 16>> m_debugger;
    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
    std::unordered_map<u8, u8>& m_outputs_during_cycle;

    Governor& m_governor;

    void change_state(std::shared_ptr<State> new_state);

    std::shared_ptr<State> paused_state();

    std::shared_ptr<State> running_state();

    std::shared_ptr<State> stepping_state();

    std::shared_ptr<State> stopped_state();

    std::shared_ptr<State> current_state();

    void set_running_state(std::shared_ptr<State> state);

    void set_paused_state(std::shared_ptr<State> state);

    void set_stepping_state(std::shared_ptr<State> state);

    void set_stopped_state(std::shared_ptr<State> state);

private:
    std::shared_ptr<State> m_current_state;
    std::shared_ptr<State> m_running_state;
    std::shared_ptr<State> m_paused_state;
    std::shared_ptr<State> m_stepping_state;
    std::shared_ptr<State> m_stopped_state;
};
}
