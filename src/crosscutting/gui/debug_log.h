#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H

#include <imgui.h>

namespace emu::util::gui {
    class DebugLog {
    public:
        DebugLog();

        void clear();

        void add_log(const char *fmt, ...) IM_FMTARGS(2);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        ImGuiTextBuffer m_buf;
        ImGuiTextFilter m_filter;
        ImVector<int> m_line_offsets;
        bool m_should_autoscroll;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H
