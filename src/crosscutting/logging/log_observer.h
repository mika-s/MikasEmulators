#pragma once

#include <string>

namespace emu::logging {

class LogObserver {
public:
    virtual ~LogObserver() = default;

    virtual void log_element_added(char const* fmt, va_list args) = 0;
};
}
