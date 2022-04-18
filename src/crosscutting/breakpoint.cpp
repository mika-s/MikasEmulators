#include "breakpoint.h"

#include <utility>

namespace emu::util::debugger {

    Breakpoint::Breakpoint(std::uint16_t address, std::string line)
            : m_address(address),
              m_line(std::move(line)) {
    }

    std::string Breakpoint::line() {
        return m_line;
    }
}
