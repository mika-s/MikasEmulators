#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H

#include <string>
#include "crosscutting/typedefs.h"

namespace emu::debugger {
    class DisassembledLine {
    public:
        explicit DisassembledLine(const std::string& raw_line);

        DisassembledLine(u16 address, std::string value);

        u16 address();

        std::string full_line();

    private:
        static constexpr int address_base = 16;

        u16 m_address;
        std::string m_full_line;

        static u16 address_from_disassembly_line(std::string line);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H
