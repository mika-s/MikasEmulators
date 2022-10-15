#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H

#include "crosscutting/debugging/debug_container.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class IoInfoPane {
    public:
        IoInfoPane();

        void attach_debug_container(DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        DebugContainer m_debug_container;
        bool m_is_debug_container_set;
    };
}
#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H
