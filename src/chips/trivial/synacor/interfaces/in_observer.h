#pragma once

#include "crosscutting/typedefs.h"

namespace emu::synacor {

class InObserver {
public:
    virtual ~InObserver() = default;

    virtual void in_requested() = 0;
};
}
