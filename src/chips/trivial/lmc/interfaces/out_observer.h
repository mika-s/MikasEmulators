#pragma once

#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"

namespace emu::lmc {

using emu::misc::UInteger;

class OutObserver {
public:
    virtual ~OutObserver() = default;

    OutObserver(const OutObserver&) = delete;
    auto operator=(const OutObserver&) -> OutObserver& = delete;
    OutObserver(OutObserver&&) = delete;
    auto operator=(OutObserver&&) -> OutObserver& = delete;

    virtual void out_changed(Data acc_reg, OutType out_type) = 0;

protected:
    OutObserver() = default;
};
}
