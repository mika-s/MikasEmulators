#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_TUI_TERMINAL_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_TUI_TERMINAL_H

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/run_status.h"
#include "ui.h"
#include <memory>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::lmc {
    class GuiObserver;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::lmc {

    using emu::lmc::GuiObserver;
    using emu::misc::RunStatus;
    using emu::lmc::Address;
    using emu::lmc::Data;

    class TuiTerminal : public Ui {

    public:
        void to_terminal() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(RunStatus run_status) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer<Address, Data> &debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_TUI_TERMINAL_H
