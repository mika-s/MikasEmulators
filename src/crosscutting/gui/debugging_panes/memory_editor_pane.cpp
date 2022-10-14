#include "imgui.h"
#include "memory_editor_pane.h"

namespace emu::gui {

    MemoryEditorPane::MemoryEditorPane() = default;

    void MemoryEditorPane::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void MemoryEditorPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        if (m_debug_container.is_memory_set()) {
            m_memory_editor.DrawContents(
                    m_debug_container.memory().value().data(),
                    m_debug_container.memory().value().size()
            );
        }

        ImGui::End();
    }
}
