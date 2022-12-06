#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H

#include "crosscutting/typedefs.h"
#include <string>

namespace emu::debugger {
    template<class A>
    class DisassembledLine;
}

namespace emu::debugger {

    template<class A>
    class Breakpoint {
    public:
        Breakpoint(A address, std::string line)
            : m_address(address),
              m_line(std::move(line)) {
        }

        explicit Breakpoint(DisassembledLine<A> line)
            : Breakpoint(line.address(), line.full_line()) {
        }

        std::string line() {
            return m_line;
        }

    private:
        [[maybe_unused]] A m_address;
        std::string m_line;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
