#pragma once

#include "applications/lmc_application/io_request.h"

namespace emu::applications::lmc {

class IoObserver {
public:
    virtual ~IoObserver() = default;

    virtual void io_changed(IoRequest request) = 0;
};
}
