#ifndef MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOGGER_H
#define MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOGGER_H

#include <cstdarg>
#include <vector>

namespace emu::logging {
    class LogObserver;
}

namespace emu::logging {

    class Logger {

    public:
        Logger();

        void add_log_observer(LogObserver &observer);

        [[maybe_unused]] void remove_log_observer(LogObserver *observer);

        void info(const char *fmt, ...);

    private:
        std::vector<LogObserver *> m_log_observers;

        void notify_log_observers(const char *fmt, va_list args);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_LOGGING_LOGGER_H
