#include "breakpoint.h"
#include "debugging/disassembled_line.h"
#include "typedefs.h"
#include <utility>

namespace emu::debugger {

    Breakpoint::Breakpoint(u16 address, std::string line)
        : m_address(address),
          m_line(std::move(line)) {
    }

    Breakpoint::Breakpoint(DisassembledLine line) : Breakpoint(line.address(), line.full_line()) {
    }

    std::string Breakpoint::line() {
        return m_line;
    }
}
