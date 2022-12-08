#pragma once

#include "crosscutting/typedefs.h"

namespace emu::lmc {

class InObserver {
public:
    virtual ~InObserver() = default;

    virtual void in_requested() = 0;
};
}
