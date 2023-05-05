#pragma once

#include "applications/game_boy/key_request.h"

namespace emu::applications::game_boy {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    virtual void key_pressed(IoRequest request) = 0;
};
}
