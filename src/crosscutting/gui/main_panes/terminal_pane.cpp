#include "terminal_pane.h"
#include "imgui.h"

namespace emu::gui {

    TerminalPane::TerminalPane() = default;

    void TerminalPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Terminal pane test");

        ImGui::End();
    }
}
