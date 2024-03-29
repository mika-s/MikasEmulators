#pragma once

#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "imgui.h"

namespace emu::gui {

using emu::debugger::DebugContainer;
using emu::util::byte::is_bit_set;
using emu::util::string::hexify;

template<class A, class D, std::size_t B>
class IoInfoPane {
public:
    IoInfoPane() = default;

    void attach_debug_container(std::shared_ptr<DebugContainer<A, D, B>> debug_container)
    {
        m_debug_container = debug_container;
        m_is_debug_container_set = true;
    }

    void draw(char const* title, bool* p_open = nullptr)
    {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        if (!m_is_debug_container_set) {
            ImGui::Text("The debug container is not provided this pane.");
        } else if (!m_debug_container->is_io_set()) {
            ImGui::Text("IO is not provided to this pane.");
        } else {
            ImGui::Text("IO:");
            ImGui::Separator();
            for (auto const& io : m_debug_container->io()) {
                const std::string name = io.name();
                bool const is_active = io.is_active();
                const D new_value = io.value();
                bool const is_divided_into_bits = io.is_divided_into_bits();

                ImGui::Text("%s", name.c_str());

                if (is_active) {
                    ImGui::SameLine(250.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::Text("x");

                    ImGui::SameLine(300.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::Text("%s", hexify(new_value).c_str());
                }

                if (is_divided_into_bits) {
                    for (auto const& bit : io.bit_names()) {
                        auto& [bit_name, bit_number] = bit;

                        ImGui::Text("  %s", bit_name.c_str());

                        if (is_active) {
                            ImGui::SameLine(300.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                            ImGui::Text(is_bit_set(new_value, bit_number) ? "x" : "");
                        }
                    }
                }
            }
        }

        ImGui::End();
    }

private:
    std::shared_ptr<DebugContainer<A, D, B>> m_debug_container;
    bool m_is_debug_container_set { false };
};
}
