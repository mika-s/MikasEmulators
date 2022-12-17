#pragma once

#include "state.h"
#include <memory>

namespace emu::applications::pacman {

class StateManager {
public:
    virtual ~StateManager() = default;

    virtual void change_state(std::shared_ptr<State> new_state) = 0;

    virtual std::shared_ptr<State> paused_state() = 0;

    virtual std::shared_ptr<State> running_state() = 0;

    virtual std::shared_ptr<State> stepping_state() = 0;

    virtual std::shared_ptr<State> stopped_state() = 0;

    virtual std::shared_ptr<State> current_state() = 0;
};

}
