#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H

#include <imgui.h>

namespace emu::util::gui {
    class DisassemblyWindow {
    public:
        DisassemblyWindow();

        void draw(const char *title, bool *p_open = nullptr);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
