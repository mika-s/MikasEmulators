#include "imgui.h"
#include "cpu_info_pane.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::gui {

    using emu::util::byte::is_bit_set;
    using emu::util::string::hexify;

    CpuInfoPane::CpuInfoPane() = default;

    void CpuInfoPane::attach_debug_container(DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void CpuInfoPane::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Registers:");
        ImGui::Separator();
        if (m_debug_container.has_alternate_registers()) {
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Main");
            ImGui::SameLine(margin_main_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Alternate");
        }
        for (const auto &reg : m_debug_container.registers()) {
            const std::string name = reg.name();
            const u8 main = reg.main();

            ImGui::Text("%s", name.c_str());
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", hexify(main).c_str());

            if (reg.is_alternate_set()) {
                const u8 alternate = reg.alternate();
                ImGui::SameLine(margin_main_right, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("%s", hexify(alternate).c_str());
            }
        }
        if (m_debug_container.is_flag_register_set()) {
            const std::string name = m_debug_container.flag_register().name();
            const u8 value = m_debug_container.flag_register().value();

            ImGui::Separator();
            ImGui::Text("%s", name.c_str());
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", hexify(value).c_str());
            ImGui::SameLine(200.0f, ImGui::GetStyle().ItemInnerSpacing.x);

            float offset = 0.0f;
            for (const auto &bit : m_debug_container.flag_register().flag_names()) {
                std::string bit_name = std::get<0>(bit);
                u8 bit_number = std::get<1>(bit);
                ImGui::Text("%s", is_bit_set(value, bit_number) ? bit_name.c_str() : "-");
                offset += 10.0f;
                ImGui::SameLine(200.0f + offset, ImGui::GetStyle().ItemInnerSpacing.x);
            }
            ImGui::NewLine();
        }
        if (m_debug_container.is_pc_set()) {
            ImGui::Separator();
            ImGui::Text("PC:");
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", hexify(m_debug_container.pc()).c_str());
        }
        if (m_debug_container.is_sp_set()) {
            ImGui::Separator();
            ImGui::Text("SP:");
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", hexify(m_debug_container.sp()).c_str());
        }
        if (m_debug_container.is_interrupted_set()) {
            ImGui::Separator();
            ImGui::Text("Interrupted:");
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%i", m_debug_container.is_interrupted());
        }
        if (m_debug_container.is_interrupt_mode_set()) {
            ImGui::Separator();
            ImGui::Text("Interrupt mode:");
            ImGui::SameLine(margin_title_right, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", m_debug_container.interrupt_mode().c_str());
        }

        ImGui::End();
    }
}
