#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INP_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INP_H

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
    class Scanner;
}

namespace emu::lmc {

    class LmcInp : public InstructionInterface {
    public:
        LmcInp();

        Data eval() override;

        static std::unique_ptr<InstructionInterface> parse(Scanner &scanner);

    private:
        static const inline Data opcode = Data(901);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INP_H
