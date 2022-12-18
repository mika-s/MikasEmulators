#pragma once

#include "applications/lmc_application/gui_request.h"

namespace emu::applications::lmc {

class UiObserver {
public:
    virtual ~UiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
