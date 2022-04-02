#include "debug_log.h"

namespace emu::util::gui {
    DebugLog::DebugLog()
            : m_should_autoscroll(true) {
        clear();
    }

    void DebugLog::clear() {
        m_buf.clear();
        m_line_offsets.clear();
        m_line_offsets.push_back(0);
    }

    void DebugLog::add_log(const char *fmt, ...) {
        int old_size = m_buf.size();
        va_list args;

        va_start(args, fmt);
        m_buf.appendfv(fmt, args);
        va_end(args);

        for (int new_size = m_buf.size(); old_size < new_size; old_size++) {
            if (m_buf[old_size] == '\n') {
                m_line_offsets.push_back(old_size + 1);
            }
        }
    }

    void DebugLog::draw(const char *title, bool *p_open) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &m_should_autoscroll);
            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }
        ImGui::SameLine();
        bool clear_button = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy_button = ImGui::Button("Copy");
        ImGui::SameLine();
        m_filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear_button) {
            clear();
        }
        if (copy_button) {
            ImGui::LogToClipboard();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char *buff = m_buf.begin();
        const char *buf_end = m_buf.end();

        if (m_filter.IsActive()) {
            for (int line_no = 0; line_no < m_line_offsets.Size; line_no++) {
                const char *line_start = buff + m_line_offsets[line_no];
                const char *line_end = (line_no + 1 < m_line_offsets.Size) ? (buff + m_line_offsets[line_no + 1] - 1)
                                                                           : buf_end;
                if (m_filter.PassFilter(line_start, line_end)) {
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
        } else {
            ImGuiListClipper clipper;
            clipper.Begin(m_line_offsets.Size);

            while (clipper.Step()) {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                    const char *line_start = buff + m_line_offsets[line_no];
                    const char *line_end = (line_no + 1 < m_line_offsets.Size) ? (buff + m_line_offsets[line_no + 1] - 1)
                                                                               : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }

            clipper.End();
        }

        ImGui::PopStyleVar();

        if (m_should_autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
