#pragma once

#include "applications/zxspectrum_48k/gui_request.h"

namespace emu::applications::zxspectrum_48k {

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
