#pragma once

#include "crosscutting/typedefs.h"

namespace emu::z80 {

class InObserver {
public:
    virtual ~InObserver() = default;

    InObserver(const InObserver&) = delete;
    auto operator=(const InObserver&) -> InObserver& = delete;
    InObserver(InObserver&&) = delete;
    auto operator=(InObserver&&) -> InObserver& = delete;

    virtual void in_requested(u16 port) = 0;

protected:
    InObserver() = default;
};
}
