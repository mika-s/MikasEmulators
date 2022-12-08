#pragma once

#include "applications/space_invaders/io_request.h"

namespace emu::applications::space_invaders {

class IoObserver {
public:
    virtual ~IoObserver() = default;

    virtual void io_changed(IoRequest request) = 0;
};
}
