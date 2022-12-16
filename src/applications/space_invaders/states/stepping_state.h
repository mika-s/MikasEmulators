#pragma once

#include "crosscutting/typedefs.h"
#include "space_invaders/gui.h"
#include "space_invaders/interfaces/state.h"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace emu::applications::space_invaders {
class CpuIo;
class GuiIo;
class Input;
class StateManager;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
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
namespace emu::misc {
class Governor;
}

namespace emu::applications::space_invaders {

using emu::i8080::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;

class SteppingState : public State {

public:
    explicit SteppingState(
        StateManager& state_manager,
        CpuIo& cpu_io,
        GuiIo& gui_io,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        std::shared_ptr<Cpu> cpu,
        EmulatorMemory<u16, u8>& memory,
        std::shared_ptr<Logger> logger,
        std::shared_ptr<Debugger<u16, 16>> debugger,
        std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container,
        std::unordered_map<u8, u8>& outputs_during_cycle);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Stepping";

    // Game loop - begin
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 2000;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    static constexpr unsigned int s_rst_1_i8080 = 0xcf;
    static constexpr unsigned int s_rst_2_i8080 = 0xd7;

    StateManager& m_state_manager;

    bool m_is_stepping_cycle { false };

    CpuIo& m_cpu_io;
    GuiIo& m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Cpu> m_cpu;

    EmulatorMemory<u16, u8>& m_memory;

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<u16, 16>> m_debugger;
    std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
    std::unordered_map<u8, u8>& m_outputs_during_cycle;

    bool await_input_and_update_debug();

    std::vector<u8> vram();
};

}
