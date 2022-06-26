#include "debugger.h"

namespace emu::util::debugger {

    void Debugger::add_breakpoint(u16 breakpoint_address, Breakpoint breakpoint) {
        m_breakpoints.insert_or_assign(breakpoint_address, std::move(breakpoint));
    }

    void Debugger::remove_breakpoint(u16 breakpoint_address) {
        m_breakpoints.erase(breakpoint_address);
    }

    std::unordered_map<u16, Breakpoint> &Debugger::breakpoints() {
        return m_breakpoints;
    }

    void Debugger::clear_all_breakpoints() {
        m_breakpoints.clear();
    }

    bool Debugger::has_breakpoint(u16 breakpoint_address) {
        return m_breakpoints.find(breakpoint_address) != m_breakpoints.end();
    }
}
