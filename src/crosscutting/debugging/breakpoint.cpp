#include "breakpoint.h"

namespace emu::util::debugger {

    Breakpoint::Breakpoint(u16 address, std::string line)
            : m_address(address),
              m_line(std::move(line)) {
    }

    std::string Breakpoint::line() {
        return m_line;
    }
}
