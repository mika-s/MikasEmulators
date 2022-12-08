#pragma once

#include "crosscutting/typedefs.h"

namespace emu::z80 {

class OutObserver {
public:
    virtual ~OutObserver() = default;

    virtual void out_changed(u8 port) = 0;
};
}
