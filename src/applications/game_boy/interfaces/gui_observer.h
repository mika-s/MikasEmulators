#pragma once

#include "applications/game_boy/gui_request.h"

namespace emu::applications::game_boy {

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
