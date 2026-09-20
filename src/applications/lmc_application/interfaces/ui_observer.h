#pragma once

#include "applications/lmc_application/gui_request.h"

namespace emu::applications::lmc {

class UiObserver {
public:
    virtual ~UiObserver() = default;

    UiObserver(const UiObserver&) = delete;
    auto operator=(const UiObserver&) -> UiObserver& = delete;
    UiObserver(UiObserver&&) = delete;
    auto operator=(UiObserver&&) -> UiObserver& = delete;

    virtual void gui_request(GuiRequest request) = 0;

protected:
    UiObserver() = default;
};
}
