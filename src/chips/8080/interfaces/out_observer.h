#pragma once

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

class OutObserver {
public:
    virtual ~OutObserver() = default;

    OutObserver(const OutObserver&) = delete;
    auto operator=(const OutObserver&) -> OutObserver& = delete;
    OutObserver(OutObserver&&) = delete;
    auto operator=(OutObserver&&) -> OutObserver& = delete;

    virtual void out_changed(u8 port) = 0;

protected:
    OutObserver() = default;
};
}
