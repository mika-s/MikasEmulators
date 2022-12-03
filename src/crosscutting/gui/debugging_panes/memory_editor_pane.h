#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H

#include "crosscutting/debugging/debug_container.h"
#include "imgui_memory_editor.h"
#include "crosscutting/typedefs.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    class MemoryEditorPane {
    public:
        MemoryEditorPane();

        void attach_debug_container(DebugContainer<u16, u8> &debug_container);

        void draw(const char *title, bool *p_open);

    private:
        MemoryEditor m_memory_editor;
        DebugContainer<u16, u8> m_debug_container;
        bool m_is_debug_container_set;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H
