#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DEBUG_LOG_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DEBUG_LOG_PANE_H

#include "imgui.h"
#include "crosscutting/logging/log_observer.h"
#include "crosscutting/logging/logger.h"

namespace emu::gui {

    using emu::logging::Logger;
    using emu::logging::LogObserver;

    class DebugLogPane : public LogObserver {
    public:
        DebugLogPane();

        void draw(const char *title, bool *p_open = nullptr);

        void clear();

        void log_element_added(const char *fmt, va_list args) override;

        void attach_logger(std::shared_ptr<Logger> logger);

    private:
        std::shared_ptr<Logger> m_logger;

        ImGuiTextBuffer m_buf;
        ImGuiTextFilter m_filter;
        ImVector<int> m_line_offsets;
        bool m_should_autoscroll;

        void add_log_with_timestamp(const char *fmt, va_list args);

        void add_log(const char *fmt, va_list args);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DEBUG_LOG_PANE_H
