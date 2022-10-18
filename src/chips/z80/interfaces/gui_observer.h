#ifndef MIKA_EMULATORS_CHIPS_Z80_INTERFACES_GUI_OBSERVER_H
#define MIKA_EMULATORS_CHIPS_Z80_INTERFACES_GUI_OBSERVER_H

#include "crosscutting/misc/run_status.h"

namespace emu::z80 {

    using emu::misc::RunStatus;

    class GuiObserver {
    public:
        virtual ~GuiObserver() = default;

        virtual void run_status_changed(RunStatus new_status) = 0;

        virtual void debug_mode_changed(bool is_in_debug_mode) = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_INTERFACES_GUI_OBSERVER_H
