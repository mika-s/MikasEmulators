#include <stdexcept>
#include "imgui.h"
#include "disassembly_window.h"

namespace emu::util::gui {

    DisassemblyWindow::DisassemblyWindow()
            : m_address_to_goto("00000000"),
              m_is_following_pc(false) {
    }

    void DisassemblyWindow::attach_debug_container(cpu8080::DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void DisassemblyWindow::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Checkbox("Follow PC", &m_is_following_pc);
        ImGui::SameLine();

        bool is_going_to_pc = false;
        if (ImGui::Button("Go to PC")) {
            is_going_to_pc = true;
        }
        ImGui::SameLine();

        bool is_going_to_address = false;
        if (ImGui::Button("Go to address")) {
            is_going_to_address = true;
            m_is_following_pc = false;
        }
        ImGui::SameLine();

        ImGui::InputText("address_to_goto", m_address_to_goto, IM_ARRAYSIZE(m_address_to_goto),
                         ImGuiInputTextFlags_CharsHexadecimal);
        std::uint16_t address_to_goto_as_number = static_cast<std::uint16_t>(std::stoi(m_address_to_goto, nullptr, address_base));

        // - Scroll when PC leaves the visible area, but not otherwise.

        const std::uint16_t pc = m_debug_container.pc();

        if (m_debug_container.is_disassembled_program_set()) {
            ImGui::BeginChild("disassembled_code_child",
                              ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
                              false,
                              ImGuiWindowFlags_HorizontalScrollbar);

            for (auto &line: m_debug_container.disassembled_program()) {
                std::uint16_t address = address_from_disassembly_line(line);

                if (m_is_following_pc && address == pc) {
                    ImGui::SetScrollHereY(0.25f);
                } else {
                    const bool is_scrolling_due_to_goto_pc = is_going_to_pc && address == pc;
                    const bool is_scrolling_due_to_given_address = is_going_to_address && address == address_to_goto_as_number;
                    if (is_scrolling_due_to_goto_pc || is_scrolling_due_to_given_address) {
                        ImGui::SetScrollHereY(0.25f);
                    }
                }

                if (pc == address) {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
                }
                if (ImGui::Selectable(line.c_str(), m_breakpoints[line], ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        m_breakpoints[line] = !m_breakpoints[line];
                    }
                }
                if (pc == address) {
                    ImGui::PopStyleColor();
                }
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    std::uint16_t DisassemblyWindow::address_from_disassembly_line(std::string line) {
        const std::string delimiter = "\t";
        size_t pos;
        std::string token;
        std::vector<std::string> split;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            split.push_back(token);
            line.erase(0, pos + delimiter.length());
        }

        if (split.empty()) {
            throw std::runtime_error("Programming error: no elements in split disassembler line");
        }

        return static_cast<std::uint16_t>(std::stoi(split[0], nullptr, address_base));
    }
}
