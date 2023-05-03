#pragma once

#include "crosscutting/typedefs.h"

namespace emu::lr35902 {

class InObserver {
public:
    virtual ~InObserver() = default;

    virtual void in_requested(u16 port) = 0;
};
}
