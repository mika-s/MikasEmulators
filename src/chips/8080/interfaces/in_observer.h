#pragma once

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

class InObserver {
public:
    virtual ~InObserver() = default;

    virtual void in_requested(u8 port) = 0;
};
}
