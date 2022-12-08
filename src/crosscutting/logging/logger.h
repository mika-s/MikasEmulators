#pragma once

#include <cstdarg>
#include <vector>

namespace emu::logging {
class LogObserver;
}

namespace emu::logging {

class Logger {

public:
    Logger();

    void add_log_observer(LogObserver& observer);

    [[maybe_unused]] void remove_log_observer(LogObserver* observer);

    void info(char const* fmt, ...);

private:
    std::vector<LogObserver*> m_log_observers;

    void notify_log_observers(char const* fmt, va_list args);
};
}
