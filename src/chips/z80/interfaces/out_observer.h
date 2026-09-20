#pragma once

#include "crosscutting/typedefs.h"

namespace emu::z80 {

class OutObserver {
public:
    virtual ~OutObserver() = default;

    OutObserver(const OutObserver&) = delete;
    auto operator=(const OutObserver&) -> OutObserver& = delete;
    OutObserver(OutObserver&&) = delete;
    auto operator=(OutObserver&&) -> OutObserver& = delete;

    virtual void out_changed(u16 port) = 0;

protected:
    OutObserver() = default;
};
}
