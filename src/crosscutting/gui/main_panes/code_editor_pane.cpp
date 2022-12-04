#include "code_editor_pane.h"
#include "imgui.h"

namespace emu::gui {

    CodeEditorPane::CodeEditorPane() = default;

    void CodeEditorPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Code editor pane test");

        ImGui::End();
    }
}
