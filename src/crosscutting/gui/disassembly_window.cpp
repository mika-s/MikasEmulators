#include <memory>
#include "imgui.h"
#include "disassembly_window.h"
#include "crosscutting/debugging/breakpoint.h"
#include "crosscutting/util/string_util.h"

namespace emu::gui {

    using emu::debugger::Breakpoint;
    using emu::util::string::hexify;

    DisassemblyWindow::DisassemblyWindow()
            : m_address_to_goto_str("00000000"),
              m_address_to_goto(0),
              m_bp_address_to_goto(0),
              m_is_following_pc(false),
              m_is_following_pc_previous(false),
              m_is_going_to_pc(false),
              m_is_going_to_address(false),
              m_is_going_to_breakpoint(false) {
    }

    void DisassemblyWindow::attach_debugger(std::shared_ptr<Debugger> debugger) {
        m_debugger = std::move(debugger);
    }

    void DisassemblyWindow::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void DisassemblyWindow::attach_logger(std::shared_ptr<Logger> logger) {
        m_logger = std::move(logger);
    }

    void DisassemblyWindow::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        reset_temp_state();
        draw_menubar();
        draw_buttons();
        draw_addresses();

        // - Scroll when PC leaves the visible area, but not otherwise.

        ImGui::End();
    }

    void DisassemblyWindow::reset_temp_state() {
        m_is_going_to_pc = false;
        m_is_going_to_address = false;
        m_is_going_to_breakpoint = false;
    }

    void DisassemblyWindow::draw_menubar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Menu")) {
                if (ImGui::BeginMenu("Breakpoints")) {
                    if (ImGui::Button("Clear all")) {
                        m_debugger->clear_all_breakpoints();
                        m_logger->info("Clearing all breakpoints");
                    }
                    ImGui::BeginChild("breakpoint_list_child",
                                      ImVec2(200, 200),
                                      false,
                                      ImGuiWindowFlags_HorizontalScrollbar);
                    if (ImGui::BeginTable("breakpoint_table", 3)) {
                        for (auto &breakpoint: m_debugger->breakpoints()) {
                            auto &[address, tooltip] = breakpoint;

                            ImGui::PushID(address);
                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("%s", hexify(address).c_str());

                            if (ImGui::IsItemHovered()) {
                                ImGui::SetTooltip("%s", tooltip.line().c_str());
                            }

                            ImGui::TableSetColumnIndex(1);
                            if (ImGui::Button("Goto")) {
                                m_is_going_to_breakpoint = true;
                                m_bp_address_to_goto = address;
                                m_is_following_pc = false;
                            }

                            ImGui::TableSetColumnIndex(2);
                            if (ImGui::Button("Delete")) {
                                m_debugger->remove_breakpoint(address);
                                m_logger->info("Removing breakpoint: 0x%04x", address);
                                ImGui::PopID();
                                break;
                            }

                            ImGui::PopID();
                        }
                        ImGui::EndTable();
                    }

                    ImGui::EndChild();

                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void DisassemblyWindow::draw_buttons() {
        ImGui::Checkbox("Follow PC", &m_is_following_pc);
        if (m_is_following_pc != m_is_following_pc_previous) {
            m_logger->info(m_is_following_pc ? "Following PC" : "Stop following PC");
        }
        m_is_following_pc_previous = m_is_following_pc;
        ImGui::SameLine();

        if (ImGui::Button("Go to PC")) {
            m_is_going_to_pc = true;
            m_logger->info("Going to PC");
        }
        ImGui::SameLine();

        if (ImGui::Button("Go to address")) {
            m_is_going_to_address = true;
            m_is_following_pc = false;
            m_logger->info("Going to address: 0x%04x", m_address_to_goto);
        }
        ImGui::SameLine();

        ImGui::InputText("address_to_goto", m_address_to_goto_str, IM_ARRAYSIZE(m_address_to_goto_str),
                         ImGuiInputTextFlags_CharsHexadecimal);
        m_address_to_goto = static_cast<u16>(std::stoi(m_address_to_goto_str, nullptr, address_base));
    }

    void DisassemblyWindow::draw_addresses() {
        if (m_debug_container.is_disassembled_program_set()) {
            const u16 pc = m_debug_container.pc();

            ImGui::BeginChild("disassembled_code_child",
                              ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
                              false,
                              ImGuiWindowFlags_HorizontalScrollbar);

            for (auto &line: m_debug_container.disassembled_program()) {
                const u16 address = line.address();
                const std::string full_line = line.full_line();
                const bool is_currently_looking_at_pc = address == pc;

                if (m_is_following_pc && is_currently_looking_at_pc) {
                    ImGui::SetScrollHereY(0.25f);
                } else {
                    const bool is_scrolling_due_to_goto_pc = m_is_going_to_pc && is_currently_looking_at_pc;
                    const bool is_scrolling_due_to_given_address = m_is_going_to_address && address == m_address_to_goto;
                    const bool is_scrolling_due_to_goto_bp = m_is_going_to_breakpoint && address == m_bp_address_to_goto;
                    if (is_scrolling_due_to_goto_pc || is_scrolling_due_to_given_address ||
                        is_scrolling_due_to_goto_bp) {
                        ImGui::SetScrollHereY(0.25f);
                    }
                }

                if (is_currently_looking_at_pc) {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                }

                const bool is_breakpoint = m_debugger->has_breakpoint(address);
                if (ImGui::Selectable(full_line.c_str(), is_breakpoint, ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        if (is_breakpoint) {
                            m_debugger->remove_breakpoint(address);
                            m_logger->info("Removing breakpoint: 0x%04x", address);
                        } else {
                            m_debugger->add_breakpoint(address, Breakpoint(line));
                            m_logger->info("Adding breakpoint: 0x%04x", address);
                        }
                    }
                }

                if (is_currently_looking_at_pc) {
                    ImGui::PopStyleColor();
                }
            }

            ImGui::EndChild();
        }
    }
}
