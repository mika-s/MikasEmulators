#include <iostream>
#include "cpu_info.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::util::gui {

    using emu::util::byte::is_bit_set;

    CpuInfo::CpuInfo() = default;

    void CpuInfo::attach_debug_container(emu::cpu8080::DebugContainer &debug_container) {
        m_debug_container = debug_container;
    }

    void CpuInfo::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Registers:");
        ImGui::Separator();
        for (const auto &reg : m_debug_container.registers()) {
            std::string name = std::get<0>(reg);
            std::uint8_t value = std::get<1>(reg)();

            ImGui::Text("%s", name.c_str());
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(value).c_str());
        }
        if (m_debug_container.is_flag_register_set()) {
            std::string name = std::get<0>(m_debug_container.flag_register());
            std::uint8_t value = std::get<1>(m_debug_container.flag_register())();

            ImGui::Separator();
            ImGui::Text("%s", name.c_str());
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(value).c_str());
            ImGui::SameLine(200.0f, ImGui::GetStyle().ItemInnerSpacing.x);

            float offset = 0.0f;
            for (const auto &bit : m_debug_container.flag_names()) {
                std::string bit_name = std::get<0>(bit);
                std::uint8_t bit_number = std::get<1>(bit);
                ImGui::Text("%s", is_bit_set(value, bit_number) ? bit_name.c_str() : "-");
                offset += 10.0f;
                ImGui::SameLine(200.0f + offset, ImGui::GetStyle().ItemInnerSpacing.x);
            }
        }
        if (m_debug_container.is_pc_set()) {
            ImGui::Separator();
            ImGui::Text("PC:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(m_debug_container.pc()).c_str());
        }
        if (m_debug_container.is_sp_set()) {
            ImGui::Separator();
            ImGui::Text("SP:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(m_debug_container.sp()).c_str());
        }
        if (m_debug_container.is_interrupted_set()) {
            ImGui::Separator();
            ImGui::Text("Interrupted:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%i", m_debug_container.is_interrupted());
        }

        ImGui::End();
    }
}
