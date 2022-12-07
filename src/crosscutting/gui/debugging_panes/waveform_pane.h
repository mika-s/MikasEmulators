#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <memory>

namespace emu::debugger {
    template<class A, class D, std::size_t B>
    class DebugContainer;
}

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class WaveformPane {
    public:
        explicit WaveformPane();

        void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr float waveform_width = 80.0f;

        std::shared_ptr<DebugContainer<u16, u8, 16>> m_debug_container;
        bool m_is_debug_container_set;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_WAVEFORM_PANE_H
