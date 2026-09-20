#pragma once

#include "applications/pacman/key_request.h"

namespace emu::applications::pacman {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    KeyObserver(const KeyObserver&) = delete;
    auto operator=(const KeyObserver&) -> KeyObserver& = delete;
    KeyObserver(KeyObserver&&) = delete;
    auto operator=(KeyObserver&&) -> KeyObserver& = delete;

    virtual void key_pressed(IoRequest request) = 0;

protected:
    KeyObserver() = default;
};
}
