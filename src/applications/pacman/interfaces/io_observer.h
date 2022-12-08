#pragma once

#include "applications/pacman/io_request.h"

namespace emu::applications::pacman {

class IoObserver {
public:
    virtual ~IoObserver() = default;

    virtual void io_changed(IoRequest request) = 0;
};
}
