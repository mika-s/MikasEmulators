#pragma once

#include "applications/game_boy/key_request.h"

namespace emu::applications::game_boy {

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
