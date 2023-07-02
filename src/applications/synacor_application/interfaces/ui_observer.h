#pragma once

#include "applications/synacor_application/gui_request.h"

namespace emu::applications::synacor {

class UiObserver {
public:
    virtual ~UiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
