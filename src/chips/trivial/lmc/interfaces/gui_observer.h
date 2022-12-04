#ifndef MIKA_EMULATORS_CHIPS_LMC_INTERFACES_GUI_OBSERVER_H
#define MIKA_EMULATORS_CHIPS_LMC_INTERFACES_GUI_OBSERVER_H

#include "crosscutting/misc/run_status.h"
#include <string>

namespace emu::lmc {

    using emu::misc::RunStatus;

    class GuiObserver {
    public:
        virtual ~GuiObserver() = default;

        virtual void run_status_changed(RunStatus new_status) = 0;

        virtual void debug_mode_changed(bool is_in_debug_mode) = 0;

        virtual void source_code_changed(const std::string &source) = 0;

        virtual void assemble_and_load_request() = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_INTERFACES_GUI_OBSERVER_H
