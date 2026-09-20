#pragma once

#include "applications/zxspectrum_48k/key_request.h"

namespace emu::applications::zxspectrum_48k {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    KeyObserver(const KeyObserver&) = delete;
    auto operator=(const KeyObserver&) -> KeyObserver& = delete;
    KeyObserver(KeyObserver&&) = delete;
    auto operator=(KeyObserver&&) -> KeyObserver& = delete;

    virtual void key_pressed(KeyRequest request) = 0;

protected:
    KeyObserver() = default;
};
}
