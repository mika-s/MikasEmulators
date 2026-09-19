#pragma once

#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "imgui.h"
#include <ostream>
#include <string>

namespace emu::gui {

using emu::debugger::DebugContainer;
using emu::util::byte::is_bit_set;
using emu::util::string::hexify;

template<class A, class D, std::size_t B>
class CpuInfoPane {
public:
    CpuInfoPane() = default;

    void attach_debug_container(std::shared_ptr<DebugContainer<A, D, B>> debug_container)
    {
        m_debug_container = std::move(debug_container);
        m_is_debug_container_set = true;
    }

    void draw(char const* title, bool* p_open = nullptr) // NOLINT(*-function-cognitive-complexity)
    {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        if (!m_is_debug_container_set) {
            ImGui::Text("The debug container is not provided this pane."); // NOLINT(*-pro-type-vararg)
        } else {
            ImGui::Text("Registers:"); // NOLINT(*-pro-type-vararg)
            ImGui::Separator();
            if (m_debug_container->has_alternate_registers()) {
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("Main"); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_main_right, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("Alternate"); // NOLINT(*-pro-type-vararg)
            }
            for (auto const& reg : m_debug_container->registers()) {
                const std::string name = reg.name();
                const D main = reg.main();

                ImGui::Text("%s", name.c_str()); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                if (m_debug_container->is_decimal()) {
                    std::stringstream ss;
                    ss << main;
                    ImGui::Text("%s", ss.str().c_str()); // NOLINT(*-pro-type-vararg)
                } else {
                    ImGui::Text("%s", hexify(main).c_str()); // NOLINT(*-pro-type-vararg)
                }

                if (reg.is_alternate_set()) {
                    const D alternate = reg.alternate();
                    ImGui::SameLine(s_margin_main_right, ImGui::GetStyle().ItemInnerSpacing.x);
                    if (m_debug_container->is_decimal()) {
                        std::stringstream ss;
                        ss << alternate;
                        ImGui::Text("%s", ss.str().c_str()); // NOLINT(*-pro-type-vararg)
                    } else {
                        ImGui::Text("%s", hexify(alternate).c_str()); // NOLINT(*-pro-type-vararg)
                    }
                }
            }
            if (m_debug_container->is_flag_register_set()) {
                const std::string name = m_debug_container->flag_register().name();
                const D value = m_debug_container->flag_register().value();

                ImGui::Separator();
                ImGui::Text("%s", name.c_str()); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                if (m_debug_container->is_decimal()) {
                    std::stringstream ss;
                    ss << value;
                    ImGui::Text("%s", ss.str().c_str()); // NOLINT(*-pro-type-vararg)
                } else {
                    ImGui::Text("%s", hexify(value).c_str()); // NOLINT(*-pro-type-vararg)
                }
                ImGui::SameLine(200.0F, ImGui::GetStyle().ItemInnerSpacing.x);

                float offset = 0.0F;
                for (auto const& bit : m_debug_container->flag_register().flag_names()) {
                    auto& [bit_name, bit_number] = bit;
                    ImGui::Text("%s", is_bit_set(value, bit_number) ? bit_name.c_str() : "-"); // NOLINT(*-pro-type-vararg)
                    offset += 10.0F;
                    ImGui::SameLine(200.0F + offset, ImGui::GetStyle().ItemInnerSpacing.x);
                }
                ImGui::NewLine();
            }
            if (m_debug_container->is_pc_set()) {
                ImGui::Separator();
                ImGui::Text("PC:"); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                if (m_debug_container->is_decimal()) {
                    std::stringstream ss;
                    ss << m_debug_container->pc();
                    ImGui::Text("%s", ss.str().c_str()); // NOLINT(*-pro-type-vararg)
                } else {
                    ImGui::Text("%s", hexify(m_debug_container->pc()).c_str()); // NOLINT(*-pro-type-vararg)
                }
            }
            if (m_debug_container->is_sp_set()) {
                ImGui::Separator();
                ImGui::Text("SP:"); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                if (m_debug_container->is_decimal()) {
                    std::stringstream ss;
                    ss << m_debug_container->sp();
                    ImGui::Text("%s", ss.str().c_str()); // NOLINT(*-pro-type-vararg)
                } else {
                    ImGui::Text("%s", hexify(m_debug_container->sp()).c_str()); // NOLINT(*-pro-type-vararg)
                }
            }
            if (m_debug_container->is_interrupted_set()) {
                ImGui::Separator();
                ImGui::Text("Interrupted:"); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("%i", m_debug_container->is_interrupted()); // NOLINT(*-pro-type-vararg)
            }
            if (m_debug_container->is_interrupt_mode_set()) {
                ImGui::Separator();
                ImGui::Text("Interrupt mode:"); // NOLINT(*-pro-type-vararg)
                ImGui::SameLine(s_margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("%s", m_debug_container->interrupt_mode().c_str()); // NOLINT(*-pro-type-vararg)
            }
        }

        ImGui::End();
    }

private:
    static constexpr float s_margin_title_right = 120.0F;
    static constexpr float s_margin_main_right = 240.0F;

    std::shared_ptr<DebugContainer<A, D, B>> m_debug_container;
    bool m_is_debug_container_set { false };
};
}
