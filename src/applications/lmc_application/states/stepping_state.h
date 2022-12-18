#pragma once

#include "applications/lmc_application/interfaces/state.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::applications::lmc {
class StateContext;
}

namespace emu::applications::lmc {

class SteppingState : public State {

public:
    explicit SteppingState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_run_awaiting_input() override;

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

    std::shared_ptr<StateContext> m_ctx;

    bool await_input_and_update_debug();
};
}
