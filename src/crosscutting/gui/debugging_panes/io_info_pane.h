#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H

#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "imgui.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;
    using emu::util::byte::is_bit_set;
    using emu::util::string::hexify;

    template<class A, class D>
    class IoInfoPane {
    public:
        IoInfoPane() = default;

        void attach_debug_container(std::shared_ptr<DebugContainer<A, D>> debug_container) {
            m_debug_container = debug_container;
            m_is_debug_container_set = true;
        }

        void draw(const char *title, bool *p_open = nullptr) {
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
                for (const auto &io: m_debug_container->io()) {
                    const std::string name = io.name();
                    const bool is_active = io.is_active();
                    const D new_value = io.value();
                    const bool is_divided_into_bits = io.is_divided_into_bits();

                    ImGui::Text("%s", name.c_str());

                    if (is_active) {
                        ImGui::SameLine(250.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                        ImGui::Text("x");

                        ImGui::SameLine(300.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                        ImGui::Text("%s", hexify(new_value).c_str());
                    }

                    if (is_divided_into_bits) {
                        for (const auto &bit: io.bit_names()) {
                            auto &[bit_name, bit_number] = bit;

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
        std::shared_ptr<DebugContainer<A, D>> m_debug_container;
        bool m_is_debug_container_set{false};
    };
}
#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_IO_INFO_PANE_H
