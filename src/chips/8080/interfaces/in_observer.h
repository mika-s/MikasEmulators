#pragma once

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

class InObserver {
public:
    virtual ~InObserver() = default;

    InObserver(const InObserver&) = delete;
    auto operator=(const InObserver&) -> InObserver& = delete;
    InObserver(InObserver&&) = delete;
    auto operator=(InObserver&&) -> InObserver& = delete;

    virtual void in_requested(u8 port) = 0;

protected:
    InObserver() = default;
};
}
