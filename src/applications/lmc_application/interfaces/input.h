#pragma once

#include "applications/lmc_application/gui_io.h"

namespace emu::applications::lmc {

class Input {
public:
    virtual ~Input() = default;

    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    Input(Input&&) = delete;
    auto operator=(Input&&) -> Input& = delete;

    virtual void read(GuiIo& gui_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;

protected:
    Input() = default;
};
}
