#pragma once

#include "applications/zxspectrum_48k/key_request.h"

namespace emu::applications::zxspectrum_48k {

class KeyObserver {
public:
    virtual ~KeyObserver() = default;

    virtual void key_pressed(KeyRequest request) = 0;
};
}
