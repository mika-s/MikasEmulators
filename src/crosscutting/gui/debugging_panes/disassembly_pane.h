#pragma once

#include "crosscutting/debugging/breakpoint.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::logging {
class Logger;
}

namespace emu::gui {

using emu::debugger::Breakpoint;
using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::logging::Logger;

template<class A, class D, std::size_t B>
class DisassemblyPane {
public:
    DisassemblyPane() = default;

    void attach_debugger(std::shared_ptr<Debugger<A, B>> debugger)
    {
        m_debugger = std::move(debugger);
        m_is_debugger_set = true;
    }

    void attach_debug_container(std::shared_ptr<DebugContainer<A, D, B>> debug_container)
    {
        m_debug_container = std::move(debug_container);
        m_is_debug_container_set = true;
    }

    void attach_logger(std::shared_ptr<Logger> logger)
    {
        m_logger = std::move(logger);
        m_is_logger_set = true;
    }

    void draw(char const* title, bool* p_open = nullptr)
    {
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        if (!m_is_debugger_set) {
            ImGui::Text("The debugger is not provided this pane.");
        } else if (!m_is_debug_container_set) {
            ImGui::Text("The debug container is not provided this pane.");
        } else if (!m_is_logger_set) {
            ImGui::Text("The logger is not provided this pane.");
        } else if (!m_debug_container->is_disassembled_program_set()) {
            ImGui::Text("Disassembled program is not provided to this pane.");
        } else {
            reset_temp_state();
            draw_menubar();
            draw_buttons();
            draw_addresses();

            // - Scroll when PC leaves the visible area, but not otherwise.
        }

        ImGui::End();
    }

private:
    static constexpr int max_address_size = 9;
    static constexpr int address_base = 16;

    std::shared_ptr<Debugger<A, B>> m_debugger;
    std::shared_ptr<DebugContainer<A, D, B>> m_debug_container;
    std::shared_ptr<Logger> m_logger;
    bool m_is_debugger_set { false };
    bool m_is_debug_container_set { false };
    bool m_is_logger_set { false };
    std::vector<std::string> m_content;

    char m_address_to_goto_str[max_address_size] { "00000000" }; // NOLINT
    A m_address_to_goto { 0 };
    A m_bp_address_to_goto { 0 };

    bool m_is_following_pc { false };
    bool m_is_following_pc_previous { false };
    bool m_is_going_to_pc { false };
    bool m_is_going_to_address { false };
    bool m_is_going_to_breakpoint { false };

    void reset_temp_state()
    {
        m_is_going_to_pc = false;
        m_is_going_to_address = false;
        m_is_going_to_breakpoint = false;
    }

    void draw_menubar()
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Menu")) {
                if (ImGui::BeginMenu("Breakpoints")) {
                    if (ImGui::Button("Clear all")) {
                        m_debugger->clear_all_breakpoints();
                        m_logger->info("Clearing all breakpoints");
                    }
                    ImGui::BeginChild("breakpoint_list_child", ImVec2(200, 200), false, ImGuiWindowFlags_HorizontalScrollbar);
                    if (ImGui::BeginTable("breakpoint_table", 3)) {
                        for (auto& breakpoint : m_debugger->breakpoints()) {
                            auto& [address, tooltip] = breakpoint;

                            std::stringstream address_ss;
                            address_ss << address;
                            ImGui::PushID(address_ss.str().c_str());
                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::AlignTextToFramePadding();
                            if (m_debug_container->is_decimal()) {
                                std::stringstream ss;
                                ss << address;
                                ImGui::Text("%s", ss.str().c_str());
                            } else {
                                ImGui::Text("%s", hexify(address).c_str());
                            }

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
                                if (m_debug_container->is_decimal()) {
                                    std::stringstream ss;
                                    ss << address;
                                    m_logger->info("Removing breakpoint: %s", ss.str().c_str());
                                } else {
                                    m_logger->info("Removing breakpoint: 0x%04x", address);
                                }
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

    void draw_buttons()
    {
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
            if (m_debug_container->is_decimal()) {
                std::stringstream ss;
                ss << m_address_to_goto;
                m_logger->info("Going to address: %s", m_address_to_goto);
            } else {
                m_logger->info("Going to address: 0x%04x", m_address_to_goto);
            }
        }
        ImGui::SameLine();

        ImGui::InputText("address_to_goto", m_address_to_goto_str, IM_ARRAYSIZE(m_address_to_goto_str), ImGuiInputTextFlags_CharsHexadecimal);
        m_address_to_goto = A(std::stoi(m_address_to_goto_str, nullptr, address_base));
    }

    void draw_addresses()
    {
        if (m_debug_container->is_disassembled_program_set()) {
            const A pc = m_debug_container->pc();

            ImGui::BeginChild(
                "disassembled_code_child",
                ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y),
                false, ImGuiWindowFlags_HorizontalScrollbar);

            for (auto& line : m_debug_container->disassembled_program()) {
                const A address = line.address();
                const std::string full_line = line.full_line();
                bool const is_currently_looking_at_pc = address == pc;

                if (m_is_following_pc && is_currently_looking_at_pc) {
                    ImGui::SetScrollHereY(0.25f);
                } else {
                    bool const is_scrolling_due_to_goto_pc = m_is_going_to_pc && is_currently_looking_at_pc;
                    bool const is_scrolling_due_to_given_address = m_is_going_to_address && address == m_address_to_goto;
                    bool const is_scrolling_due_to_goto_bp = m_is_going_to_breakpoint && address == m_bp_address_to_goto;
                    if (is_scrolling_due_to_goto_pc || is_scrolling_due_to_given_address || is_scrolling_due_to_goto_bp) {
                        ImGui::SetScrollHereY(0.25f);
                    }
                }

                if (is_currently_looking_at_pc) {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                }

                bool const is_breakpoint = m_debugger->has_breakpoint(address);
                if (ImGui::Selectable(full_line.c_str(), is_breakpoint, ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        if (is_breakpoint) {
                            m_debugger->remove_breakpoint(address);
                            if (m_debug_container->is_decimal()) {
                                std::stringstream ss;
                                ss << m_address_to_goto;
                                m_logger->info("Removing breakpoint: %s", m_address_to_goto);
                            } else {
                                m_logger->info("Removing breakpoint: 0x%04x", address);
                            }
                        } else {
                            m_debugger->add_breakpoint(address, Breakpoint<A, B>(line));
                            if (m_debug_container->is_decimal()) {
                                std::stringstream ss;
                                ss << m_address_to_goto;
                                m_logger->info("Adding breakpoint: %s", m_address_to_goto);
                            } else {
                                m_logger->info("Adding breakpoint: 0x%04x", address);
                            }
                        }
                    }
                }

                if (is_currently_looking_at_pc) {
                    ImGui::PopStyleColor();
                }
            }

            ImGui::EndChild();
        }
    };
};
}
