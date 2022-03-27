#include <iostream>
#include "cpu_info.h"
#include "crosscutting/string_util.h"

namespace emu::util::gui {
    CpuInfo::CpuInfo() = default;

    void CpuInfo::attach_debug_container(emu::cpu8080::DebugContainer &debug_container_to_attach) {
        this->debug_container = debug_container_to_attach;
    }

    void CpuInfo::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        ImGui::Text("Registers:");
        ImGui::Separator();

        for (auto &reg : debug_container.get_registers()) {
            std::string name = std::get<0>(reg);
            std::uint8_t value = std::get<1>(reg)();

            ImGui::Text("%s", name.c_str());
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(value).c_str());
        }
        if (debug_container.is_pc_set()) {
            ImGui::Separator();
            ImGui::Text("PC:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(debug_container.get_pc()).c_str());
        }
        if (debug_container.is_sp_set()) {
            ImGui::Separator();
            ImGui::Text("SP:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%s", emu::util::string::hexify(debug_container.get_sp()).c_str());
        }
        if (debug_container.is_interrupted_set()) {
            ImGui::Separator();
            ImGui::Text("Interrupted:");
            ImGui::SameLine(100.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("%i", debug_container.get_is_interrupted());
        }

        ImGui::End();
    }
}
