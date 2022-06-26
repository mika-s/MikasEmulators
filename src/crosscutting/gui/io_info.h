#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H

#include "8080/debug_container.h"

namespace emu::util::gui {

    class IoInfo {
    public:
        IoInfo();

        void attach_debug_container(cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        cpu8080::DebugContainer m_debug_container;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_IO_INFO_H
