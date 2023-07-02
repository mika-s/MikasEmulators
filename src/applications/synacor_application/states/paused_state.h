#pragma once

#include "applications/synacor_application/interfaces/state.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::applications::synacor {
class StateContext;
}

namespace emu::applications::synacor {

class PausedState : public State {

public:
    explicit PausedState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_run_awaiting_input() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Paused";

    std::shared_ptr<StateContext> m_ctx;
};
}
