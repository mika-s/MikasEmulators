#include "lmc_memory_editor.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/misc/uinteger.h"
#include "imgui.h"
#include <cstring>
#include <fmt/core.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace emu::applications::lmc {

using emu::debugger::DebugContainer;
using emu::lmc::Address;
using emu::lmc::Data;
using emu::misc::UInteger;

void LmcMemoryEditor::attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container)
{
    m_debug_container = std::move(debug_container);
    m_is_debug_container_set = true;
}

void LmcMemoryEditor::draw(char const* title, bool* p_open)
{
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    if (!m_is_debug_container_set) {
        ImGui::Text("The debug container is not provided this pane.");
    } else if (!m_debug_container->is_memory_set()) {
        ImGui::Text("Memory is not provided to this pane.");
    } else {
        const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg //
            | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX;

        std::vector<Data> memory = m_debug_container->memory().value();

        if (ImGui::BeginTable("memory_editor", s_cols, flags)) {
            unsigned int address = 0;
            for (std::size_t col = 0; col < s_cols; ++col) {
                ImGui::TableSetupColumn(fmt::format("##lme-col-ts{}", col).c_str(), ImGuiTableColumnFlags_WidthFixed, s_box_width);
            }
            for (std::size_t row = 0; row < s_rows; ++row) {
                ImGui::TableNextRow();
                for (std::size_t col = 0; col < s_cols; ++col) {
                    ImGui::TableSetColumnIndex(col);
                    strncpy(m_values[address], fmt::format("{}", memory[address].underlying()).c_str(), s_max_chars);
                    ImGui::InputText(fmt::format("{}", address).c_str(), m_values[address], //
                        IM_ARRAYSIZE(m_values[address]), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ReadOnly);
                    //                        if (ImGui::IsItemDeactivatedAfterEdit()) {
                    //                            std::cout << "Modified\n";
                    //                        }
                    ++address;
                }
            }
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
}
