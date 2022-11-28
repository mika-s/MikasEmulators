#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_ADD_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_ADD_H

#include "instruction_interface.h"
#include "lmc_operand.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
    class Scanner;
}
namespace emu::lmc {
    class Environment;
}

namespace emu::lmc {

    class LmcAdd : public InstructionInterface {

    public:
        explicit LmcAdd(LmcOperand operand);

        Data eval() override;

        static std::unique_ptr<InstructionInterface> parse(Scanner &scanner, Environment &environment);

    private:
        static const inline Data opcode = Data(100);

        LmcOperand m_operand;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_ADD_H
