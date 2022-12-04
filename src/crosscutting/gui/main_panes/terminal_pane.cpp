#include "terminal_pane.h"
#include "imgui.h"

namespace emu::gui {

    TerminalPane::TerminalPane() = default;

    void TerminalPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const ImGuiInputTextFlags output_flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
        ImGui::InputTextMultiline("##output", output_buffer, IM_ARRAYSIZE(output_buffer), //
                                  ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 30), output_flags);

        const ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_CharsDecimal;
        ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE(input_buffer), input_flags);

        ImGui::EndChild();

        ImGui::End();
    }
}
