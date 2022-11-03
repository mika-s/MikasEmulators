#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H

#include <string>
#include <vector>
#include "imgui.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debug_container.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class WaveformPane {
    public:
        explicit WaveformPane();

        void attach_debug_container(DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr float waveform_width = 80.0f;

        DebugContainer m_debug_container;
        bool m_is_debug_container_set;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H