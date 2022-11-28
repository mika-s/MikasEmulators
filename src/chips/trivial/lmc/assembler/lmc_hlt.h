#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_HLT_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_HLT_H

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
    class Scanner;
}

namespace emu::lmc {

    class LmcHlt : public InstructionInterface {
    public:
        LmcHlt();

        Data eval() override;

        static std::unique_ptr<InstructionInterface> parse(Scanner &scanner);

    private:
        static const inline Data opcode = Data(0);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_HLT_H
