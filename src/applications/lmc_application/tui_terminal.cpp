#include "tui_terminal.h"
#include "ui.h"

namespace emu::applications::lmc {

    void TuiTerminal::to_terminal() {

    }

    void TuiTerminal::add_gui_observer([[maybe_unused]] GuiObserver &observer) {
    }

    void TuiTerminal::remove_gui_observer([[maybe_unused]] GuiObserver *observer) {
    }

    void TuiTerminal::attach_debugger([[maybe_unused]] std::shared_ptr<Debugger> debugger) {
    }

    void TuiTerminal::attach_debug_container([[maybe_unused]] DebugContainer &debug_container) {
    }

    void TuiTerminal::attach_logger([[maybe_unused]] std::shared_ptr<Logger> logger) {
    }

    void TuiTerminal::update_screen([[maybe_unused]] RunStatus run_status) {
    }

    void TuiTerminal::update_debug_only() {
    }
}
