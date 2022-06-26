#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H

#include <unordered_map>
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/breakpoint.h"

namespace emu::util::debugger {

    class Debugger {
    public:
        void add_breakpoint(u16 breakpoint_address, Breakpoint breakpoint);

        void remove_breakpoint(u16 breakpoint_address);

        std::unordered_map<u16, Breakpoint> &breakpoints();

        void clear_all_breakpoints();

        bool has_breakpoint(u16 breakpoint_address);

    private:
        std::unordered_map<u16, Breakpoint> m_breakpoints;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DEBUGGER_H
