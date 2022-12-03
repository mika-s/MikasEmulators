#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H

#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/typedefs.h"
#include "imgui_memory_editor.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;

    template<class A, class D>
    class MemoryEditorPane {
    public:
        MemoryEditorPane() = default;

        void attach_debug_container(DebugContainer<A, D> &debug_container) {
            m_debug_container = debug_container;
            m_is_debug_container_set = true;
        }

        void draw(const char *title, bool *p_open) {
            if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
                ImGui::End();
                return;
            }

            if (!m_is_debug_container_set) {
                ImGui::Text("The debug container is not provided this pane.");
            } else if (!m_debug_container.is_memory_set()) {
                ImGui::Text("Memory is not provided to this pane.");
            } else {
                m_memory_editor.DrawContents(
                        m_debug_container.memory().value().data(),
                        m_debug_container.memory().value().size()
                );
            }

            ImGui::End();
        }

    private:
        MemoryEditor m_memory_editor;
        DebugContainer<A, D> m_debug_container;
        bool m_is_debug_container_set{false};
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_MEMORY_EDITOR_PANE_H
