#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_INTERFACE_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_INTERFACE_H

#include "scanner.h"
#include "usings.h"

namespace emu::lmc {

    class InstructionInterface {
    public:
        virtual ~InstructionInterface() = default;

        virtual Data eval() = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_INTERFACE_H
