#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::space_invaders {

class State {
public:
    virtual ~State() = default;

    State(const State&) = delete;
    auto operator=(const State&) -> State& = delete;
    State(State&&) = delete;
    auto operator=(State&&) -> State& = delete;

    virtual auto is_exit_state() -> bool = 0;

    virtual void transition_to_run() = 0;

    virtual void transition_to_pause() = 0;

    virtual void transition_to_stop() = 0;

    virtual void transition_to_step() = 0;

    virtual void perform(cyc& cycles) = 0;

protected:
    State() = default;
};

}
