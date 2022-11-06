#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BASIC_BLOCK_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BASIC_BLOCK_H

#include "disassembled_line.h"
#include <vector>

namespace emu::debugger {

    class BasicBlock {
    public:
        BasicBlock();

        explicit BasicBlock(std::vector<DisassembledLine> lines);

    private:
        std::vector<DisassembledLine> m_lines;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_BASIC_BLOCK_H
