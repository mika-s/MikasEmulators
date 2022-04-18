#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGER_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGER_H

#include <cstdint>
#include <unordered_map>
#include "crosscutting/breakpoint.h"

namespace emu::util::debugger {

    class Debugger {
    public:
        void add_breakpoint(std::uint16_t breakpoint_address, Breakpoint breakpoint);

        void remove_breakpoint(std::uint16_t breakpoint_address);

        std::unordered_map<std::uint16_t, Breakpoint> breakpoints();

        void clear_all_breakpoints();

        bool has_breakpoint(std::uint16_t breakpoint_address);

    private:
        std::unordered_map<std::uint16_t, Breakpoint> m_breakpoints;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGER_H
