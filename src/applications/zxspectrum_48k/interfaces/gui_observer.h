#pragma once

#include "applications/zxspectrum_48k/gui_request.h"

namespace emu::applications::zxspectrum_48k {

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    virtual void gui_request(GuiRequest request) = 0;
};
}
