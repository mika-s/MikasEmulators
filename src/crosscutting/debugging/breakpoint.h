#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H

#include "crosscutting/typedefs.h"
#include <string>

namespace emu::debugger {
    class DisassembledLine;
}

namespace emu::debugger {

    class Breakpoint {
    public:
        Breakpoint(u16 address, std::string line);

        explicit Breakpoint(DisassembledLine line);

        std::string line();

    private:
        [[maybe_unused]] u16 m_address;
        std::string m_line;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
