#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_CPU_INFO_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_CPU_INFO_PANE_H

#include "crosscutting/debugging/debug_container.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class CpuInfoPane {
    public:
        CpuInfoPane();

        void attach_debug_container(DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr float margin_title_right = 120.0f;
        static constexpr float margin_main_right = 240.0f;

        DebugContainer m_debug_container;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_CPU_INFO_PANE_H