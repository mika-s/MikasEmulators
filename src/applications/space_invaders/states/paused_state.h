#pragma once

#include "crosscutting/typedefs.h"
#include "space_invaders/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::space_invaders {
class CpuIo;
class Gui;
class GuiIo;
class Input;
class StateManager;
}
namespace emu::i8080 {
class Cpu;
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

class PausedState : public State {

public:
    explicit PausedState(
        StateManager& state_manager,
        CpuIo& cpu_io,
        GuiIo& gui_io,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        EmulatorMemory<u16, u8>& memory,
        Governor& governor);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Paused";

    StateManager& m_state_manager;

    CpuIo& m_cpu_io;
    GuiIo& m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;

    EmulatorMemory<u16, u8>& m_memory;

    Governor& m_governor;

    std::vector<u8> vram();
};

}
