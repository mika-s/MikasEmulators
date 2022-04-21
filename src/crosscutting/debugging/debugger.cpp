#include "debugger.h"

namespace emu::util::debugger {

    void Debugger::add_breakpoint(std::uint16_t breakpoint_address, Breakpoint breakpoint) {
        m_breakpoints.insert_or_assign(breakpoint_address, std::move(breakpoint));
    }

    void Debugger::remove_breakpoint(std::uint16_t breakpoint_address) {
        m_breakpoints.erase(breakpoint_address);
    }

    std::unordered_map<std::uint16_t, Breakpoint> &Debugger::breakpoints() {
        return m_breakpoints;
    }

    void Debugger::clear_all_breakpoints() {
        m_breakpoints.clear();
    }

    bool Debugger::has_breakpoint(std::uint16_t breakpoint_address) {
        return m_breakpoints.find(breakpoint_address) != m_breakpoints.end();
    }
}
