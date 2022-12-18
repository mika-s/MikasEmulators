#pragma once

#include "applications/pacman/key_request.h"

namespace emu::applications::pacman {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    virtual void key_pressed(IoRequest request) = 0;
};
}
