#ifndef MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOG_LISTENER_H
#define MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOG_LISTENER_H

#include <string>

namespace emu::logging {

    class LogObserver {
    public:
        virtual ~LogObserver() = default;

        virtual void log_element_added(const char *fmt, va_list args) = 0;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOG_LISTENER_H
