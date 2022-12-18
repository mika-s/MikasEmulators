#pragma once

#include "applications/pacman/gui_request.h"

namespace emu::applications::pacman {

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
