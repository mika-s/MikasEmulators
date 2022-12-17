#pragma once

#include "crosscutting/typedefs.h"
#include "pacman/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::pacman {
class Gui;
class GuiIo;
class Input;
class MemoryMappedIoForPacman;
class StateManager;
}
namespace emu::z80 {
class Cpu;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::misc {
class Governor;
}

namespace emu::applications::pacman {

using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::z80::Cpu;

class PausedState : public State {

public:
    explicit PausedState(
        StateManager& state_manager,
        GuiIo& gui_io,
        std::shared_ptr<Gui> gui,
        std::shared_ptr<Input> input,
        EmulatorMemory<u16, u8>& memory,
        std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
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

    GuiIo& m_gui_io;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;

    EmulatorMemory<u16, u8>& m_memory;
    std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;

    Governor& m_governor;

    std::vector<u8> tile_ram();

    std::vector<u8> sprite_ram();

    std::vector<u8> palette_ram();
};

}
