#include <imgui.h>
#include "io_info.h"
#include "crosscutting/string_util.h"

namespace emu::util::gui {

    using emu::util::string::hexify;

    IoInfo::IoInfo() = default;

    void IoInfo::attach_debug_container(emu::cpu8080::DebugContainer &debug_container) {
        m_debug_container = debug_container;
        for (auto &io : debug_container.io()) {
            const std::string name = io.name();
            saved_values[name] = std::make_tuple(0, 0);
        }
    }

    void IoInfo::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("IO:");
        ImGui::Separator();
        if (m_debug_container.is_io_set()) {
            for (const auto &io : m_debug_container.io()) {
                const std::string name = io.name();
                const bool is_active = io.is_active();
                const std::uint8_t new_value = io.value();

                auto &[cycles_kept, kept_value] = saved_values[name];
                cycles_kept++;

                if (is_active) {
                    cycles_kept = 0;
                    kept_value = new_value;
                }

                ImGui::Text("%s", name.c_str());
                if (is_active) {
                    ImGui::SameLine(250.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::Text("x");
                }

                bool is_timed_out = cycles_kept > cycles_to_keep_value;
                if (!is_timed_out) {
                    ImGui::SameLine(300.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::Text("%s", hexify(kept_value).c_str());
                }
            }
        }

        ImGui::End();
    }
}
