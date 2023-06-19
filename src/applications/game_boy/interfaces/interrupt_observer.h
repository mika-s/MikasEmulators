#pragma once

#include "applications/game_boy/interrupts.h"

namespace emu::applications::game_boy {

class InterruptObserver {
public:
    virtual ~InterruptObserver() = default;

    virtual void interrupt(Interrupts interrupt) = 0;
};
}
