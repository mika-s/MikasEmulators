#pragma once

#include "crosscutting/misc/run_status.h"

namespace emu::i8080 {

using emu::misc::RunStatus;

class GuiObserver {
public:
    virtual ~GuiObserver() = default;

    virtual void run_status_changed(RunStatus new_status) = 0;

    virtual void debug_mode_changed(bool is_in_debug_mode) = 0;
};
}
