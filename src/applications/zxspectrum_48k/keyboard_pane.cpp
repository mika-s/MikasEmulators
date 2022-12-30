#include "keyboard_pane.h"
#include "cpu_io.h"
#include "crosscutting/util/byte_util.h"
#include "imgui.h"
#include <ext/alloc_traits.h>
#include <fmt/core.h>

namespace emu::applications::zxspectrum_48k {

using emu::util::byte::is_bit_set;

void KeyboardPane::attach_cpu_io(CpuIo const* cpu_io)
{
    m_cpu_io = cpu_io;
    m_is_cpu_io_set = true;
}

void KeyboardPane::draw(char const* title, bool* p_open)
{
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (!m_is_cpu_io_set) {
        ImGui::Text("CpuIo is not provided this pane.");
    } else {
        const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg //
            | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX;

        if (ImGui::BeginTable("keyboard", s_cols, flags)) {
            for (std::size_t row = 0; row < s_rows; ++row) {
                ImGui::TableNextRow();
                const u16 address = s_index_to_address.at(row);
                const u8 keyboard_values = m_cpu_io->keyboard_input(address);
                for (std::size_t col = 0; col < s_cols; ++col) {
                    ImGui::TableSetColumnIndex(col);

                    if (col > 0 && !is_bit_set(keyboard_values, col - 1)) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f)));
                    }

                    const std::string label = fmt::format("{}", s_keyboard_labels[row][col]);
                    ImGui::Text(
                        label.c_str(),
                        label.c_str(),
                        label.size(),
                        ImGuiInputTextFlags_ReadOnly);
                }
            }
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
}
