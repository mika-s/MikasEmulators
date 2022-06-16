#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_MEMORY_EDITOR_WINDOW_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_MEMORY_EDITOR_WINDOW_H

#include <string>
#include <vector>
#include "imgui_memory_editor.h"
#include "8080/debug_container.h"

namespace emu::util::gui {

    class MemoryEditorWindow {
    public:
        MemoryEditorWindow();

        void attach_debug_container(emu::cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open);

    private:
        MemoryEditor m_memory_editor;
        emu::cpu8080::DebugContainer m_debug_container;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_MEMORY_EDITOR_WINDOW_H
