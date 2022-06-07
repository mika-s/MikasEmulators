#include <algorithm>
#include <cstdarg>
#include "crosscutting/string_util.h"
#include "logger.h"

namespace emu::util::logging {

    using emu::util::string::prepend;

    Logger::Logger() = default;

    void Logger::add_log_observer(LogObserver &observer) {
        m_log_observers.push_back(&observer);
    }

    [[maybe_unused]] void Logger::remove_log_observer(LogObserver *observer) {
        m_log_observers.erase(
                std::remove(m_log_observers.begin(), m_log_observers.end(), observer),
                m_log_observers.end()
        );
    }

    void Logger::info(const char *fmt, ...) {
        va_list args;

        va_start(args, fmt);
        notify_log_observers(prepend("[INFO] ", fmt).c_str(), args);
        va_end(args);
    }

    void Logger::notify_log_observers(const char *fmt, va_list args) {
        for (LogObserver *observer: m_log_observers) {
            observer->log_element_added(fmt, args);
        }
    }
}
