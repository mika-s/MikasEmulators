#pragma once

#include "crosscutting/typedefs.h"
#include "pacman/interfaces/state.h"
#include <string>

namespace emu::applications::pacman {
class StateManager;
}

namespace emu::applications::pacman {

class StoppedState : public State {

public:
    explicit StoppedState(StateManager& state_manager);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Stopped";

    [[maybe_unused]] StateManager& m_state_manager;
};

}
