#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::game_boy {

class State {
public:
    virtual ~State() = default;

    virtual bool is_exit_state() = 0;

    virtual void transition_to_run() = 0;

    virtual void transition_to_pause() = 0;

    virtual void transition_to_stop() = 0;

    virtual void transition_to_step() = 0;

    virtual void perform(cyc& cycles) = 0;
};

}
