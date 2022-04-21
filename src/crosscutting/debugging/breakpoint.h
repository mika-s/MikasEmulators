#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H

#include <cstdint>
#include <string>

namespace emu::util::debugger {

    class Breakpoint {
    public:
        Breakpoint(std::uint16_t address, std::string line);

        std::string line();

    private:
        std::uint16_t m_address;
        std::string m_line;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BREAKPOINT_H
