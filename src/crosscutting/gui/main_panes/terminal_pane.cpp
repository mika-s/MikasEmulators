#include "terminal_pane.h"
#include "imgui.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

namespace emu::gui {

TerminalPane::TerminalPane() = default;

void TerminalPane::add_pane_observer(TerminalPaneObserver& observer)
{
    m_pane_observers.push_back(&observer);
}

void TerminalPane::remove_pane_observer(TerminalPaneObserver* observer)
{
    m_pane_observers.erase(
        std::remove(m_pane_observers.begin(), m_pane_observers.end(), observer),
        m_pane_observers.end());
}

void TerminalPane::draw(char const* title, bool is_awaiting_input, std::vector<std::string> const& output, bool* p_open)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    vector_to_output_array(output);
    const ImGuiInputTextFlags output_flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
    ImGui::InputTextMultiline("##output", m_output_buffer, IM_ARRAYSIZE(m_output_buffer), //
        ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 30), output_flags);

    if (is_awaiting_input) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 128, 0, 128));
    }

    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;
    if (!is_awaiting_input) {
        input_flags |= ImGuiInputTextFlags_ReadOnly;
    }
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
        ImGui::SetKeyboardFocusHere(0);
    }
    if (ImGui::InputText("Input", m_input_buffer, IM_ARRAYSIZE(m_input_buffer), input_flags)) {
        if (strlen(m_input_buffer) != 0) {
            notify_pane_observers_about_new_input();
            strcpy(m_input_buffer, "");
        }
    }

    if (is_awaiting_input) {
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();

    ImGui::End();
}

void TerminalPane::notify_pane_observers_about_new_input()
{
    for (TerminalPaneObserver* observer : m_pane_observers) {
        observer->input_sent(std::string(m_input_buffer));
    }
}

void TerminalPane::vector_to_output_array(std::vector<std::string> const& output)
{
    int i = 0;
    for (std::string const& element : output) {
        for (char ch : element) {
            m_output_buffer[i++] = ch;
        }
    }
    m_output_buffer[i] = '\0';
}
}
