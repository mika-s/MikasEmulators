#include "imgui.h"
#include "terminal_pane.h"
#include <cstddef>
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
    std::erase(m_pane_observers, observer);
}

void TerminalPane::draw(
    char const* title,
    const bool is_awaiting_input,
    std::vector<std::string> const& output,
    bool* p_open
)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("scrolling", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar);

    vector_to_output_array(output);
    constexpr ImGuiInputTextFlags output_flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
    ImGui::InputTextMultiline("##output", m_output_buffer.data(), m_output_buffer.size(),
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
    if (ImGui::InputText("Input", m_input_buffer.data(), m_input_buffer.size(), input_flags)) {
        if (!m_input_buffer.empty()) {
            notify_pane_observers_about_new_input();
            strcpy(m_input_buffer.data(), "");
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
        observer->input_sent(std::string(m_input_buffer.data()));
    }
}

void TerminalPane::vector_to_output_array(std::vector<std::string> const& output)
{
    std::size_t i = 0;
    constexpr std::size_t max_length = output_buffer_size - 1;

    for (std::string const& element : output) {
        for (char const ch : element) {
            if (i >= max_length) {
                m_output_buffer.at(i) = '\0';
                return;
            }

            m_output_buffer.at(i++) = ch;
        }
    }

    m_output_buffer.at(i) = '\0';
}
}
