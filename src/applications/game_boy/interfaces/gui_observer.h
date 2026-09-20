#pragma once

#include "applications/game_boy/gui_request.h"

namespace emu::applications::game_boy {

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    GuiObserver(const GuiObserver&) = delete;
    auto operator=(const GuiObserver&) -> GuiObserver& = delete;
    GuiObserver(GuiObserver&&) = delete;
    auto operator=(GuiObserver&&) -> GuiObserver& = delete;

    virtual void gui_request(GuiRequest request) = 0;

protected:
    GuiObserver() = default;
};
}
