#pragma once

#include "applications/space_invaders/key_request.h"

namespace emu::applications::space_invaders {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    virtual void key_pressed(KeyRequest request) = 0;
};
}
