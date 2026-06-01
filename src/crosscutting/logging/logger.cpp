#include "logger.h"
#include "crosscutting/logging/log_observer.h"
#include "crosscutting/util/string_util.h"
#include <algorithm>
#include <cstdarg>
#include <string>

namespace emu::logging {

using emu::util::string::prepend;

Logger::Logger() = default;

void Logger::add_log_observer(LogObserver& observer)
{
    m_log_observers.push_back(&observer);
}

[[maybe_unused]] void Logger::remove_log_observer(LogObserver* observer)
{
    std::erase(m_log_observers, observer);
}

void Logger::info(char const* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    notify_log_observers(prepend("[INFO] ", fmt).c_str(), args);
    va_end(args);
}

void Logger::notify_log_observers(char const* fmt, va_list args) const {
    for (LogObserver* observer : m_log_observers) {
        observer->log_element_added(fmt, args);
    }
}
}
