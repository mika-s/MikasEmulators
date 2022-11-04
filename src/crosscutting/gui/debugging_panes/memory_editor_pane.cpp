#include "memory_editor_pane.h"
#include "debugging/debug_container.h"
#include "imgui.h"
#include <vector>

namespace emu::gui {

    MemoryEditorPane::MemoryEditorPane()
        : m_is_debug_container_set(false) {
    }

    void MemoryEditorPane::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
        m_is_debug_container_set = true;
    }

    void MemoryEditorPane::draw(const char *title, bool *p_open) {
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
}
