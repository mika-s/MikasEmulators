#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"

namespace emu::synacor {

using emu::misc::UInteger;

class OutObserver {
public:
    virtual ~OutObserver() = default;

    virtual void out_changed(Data acc_reg) = 0;
};
}
