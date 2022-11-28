#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_H

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
    class Scanner;
}
namespace emu::lmc {
    class Environment;
}

namespace emu::lmc {
    class LmcInstruction {
    public:
        explicit LmcInstruction(std::unique_ptr<InstructionInterface> instruction);

        Data eval();

        static std::unique_ptr<LmcInstruction> parse(Scanner &scanner, Environment &environment);

    private:
        std::unique_ptr<InstructionInterface> m_instruction;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INSTRUCTION_H
