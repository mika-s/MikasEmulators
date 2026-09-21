#pragma once

#include <string>

namespace emu::logging {

class LogObserver {
public:
    virtual ~LogObserver() = default;

    LogObserver(const LogObserver&) = delete;
    auto operator=(const LogObserver&) -> LogObserver& = delete;
    LogObserver(LogObserver&&) = delete;
    auto operator=(LogObserver&&) -> LogObserver& = delete;

    virtual void log_element_added(char const* fmt, va_list args) = 0;

protected:
    LogObserver() = default;
};
}
