#pragma once

#include "applications/synacor_application/interfaces/state.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string_view>

namespace emu::applications::synacor {
class StateContext;
}

namespace emu::applications::synacor {

class RunningAwaitingInputState : public State {

public:
    explicit RunningAwaitingInputState(std::shared_ptr<StateContext> state_context);

    auto is_exit_state() -> bool override;

    void transition_to_run() override;

    void transition_to_run_awaiting_input() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static constexpr std::string_view s_game_window_subtitle = "";

    // Game loop - begin
    static constexpr double s_fps = 60.0;
    static constexpr double s_tick_limit = 1000.0 / s_fps;
    static constexpr int s_cycles_per_ms = 2000;
    static constexpr double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    std::shared_ptr<StateContext> m_ctx;
};
}
