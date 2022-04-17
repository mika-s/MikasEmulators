#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H

#include <imgui.h>

namespace emu::util::gui {
    class DebugLog {
    public:
        DebugLog();

        void clear();

        void add_log_with_timestamp(const char *fmt, ...) IM_FMTARGS(2);

        void add_log(const char *fmt, ...) IM_FMTARGS(2);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        ImGuiTextBuffer m_buf;
        ImGuiTextFilter m_filter;
        ImVector<int> m_line_offsets;
        bool m_should_autoscroll;

        void add_log(const char *fmt, va_list args);

        static std::string prepend(std::string prefix, const char *txt);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUG_LOG_H
