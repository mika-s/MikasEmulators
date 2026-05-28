#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::zxspectrum_48k {

class State {
public:
    virtual ~State() = default;

    virtual auto is_exit_state() -> bool = 0;

    virtual void transition_to_run() = 0;

    virtual void transition_to_pause() = 0;

    virtual void transition_to_stop() = 0;

    virtual void transition_to_step() = 0;

    virtual void perform(cyc& cycles) = 0;
};

}
