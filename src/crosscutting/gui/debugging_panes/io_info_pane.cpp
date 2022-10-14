#include "imgui.h"
#include "io_info_pane.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::gui {

    using emu::debugger::DebugContainer;
    using emu::util::byte::is_bit_set;
    using emu::util::string::hexify;

    IoInfoPane::IoInfoPane() = default;

    void IoInfoPane::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void IoInfoPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("IO:");
        ImGui::Separator();
        if (m_debug_container.is_io_set()) {
            for (const auto &io: m_debug_container.io()) {
                const std::string name = io.name();
                const bool is_active = io.is_active();
                const u8 new_value = io.value();
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
}
