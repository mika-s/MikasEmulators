#include "imgui.h"
#include "memory_editor_window.h"

namespace emu::gui {

    MemoryEditorWindow::MemoryEditorWindow() = default;

    void MemoryEditorWindow::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void MemoryEditorWindow::draw(const char *title, bool *p_open) {
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
