#pragma once

#include "applications/lmc_application/gui_io.h"

namespace emu::applications::lmc {

class Input {
public:
    virtual ~Input() = default;

    virtual void read(GuiIo& gui_io) = 0;

    virtual void read_debug_only(GuiIo& gui_io) = 0;
};
}
