#pragma once

#include "applications/space_invaders/gui_request.h"

namespace emu::applications::space_invaders {

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
