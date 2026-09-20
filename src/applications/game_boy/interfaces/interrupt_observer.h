#pragma once

#include "applications/game_boy/interrupts.h"

namespace emu::applications::game_boy {

class InterruptObserver {
public:
    virtual ~InterruptObserver() = default;

    InterruptObserver(const InterruptObserver&) = delete;
    auto operator=(const InterruptObserver&) -> InterruptObserver& = delete;
    InterruptObserver(InterruptObserver&&) = delete;
    auto operator=(InterruptObserver&&) -> InterruptObserver& = delete;

    virtual void interrupt(Interrupts interrupt) = 0;

protected:
    InterruptObserver() = default;
};
}
