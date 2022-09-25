#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H

#include "crosscutting/debugging/debug_container.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class IoInfo {
    public:
        IoInfo();

        void attach_debug_container(DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        DebugContainer m_debug_container;
    };
}
#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
