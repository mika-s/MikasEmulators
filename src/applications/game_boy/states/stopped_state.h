#pragma once

#include "crosscutting/typedefs.h"
#include "game_boy/interfaces/state.h"
#include <memory>
#include <string>

namespace emu::applications::game_boy {
class StateContext;
}

namespace emu::applications::game_boy {

class StoppedState : public State {

public:
    explicit StoppedState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Stopped";

    [[maybe_unused]] std::shared_ptr<StateContext> m_ctx;
};

}
