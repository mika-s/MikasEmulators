#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_OUT_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_OUT_H

#include "instruction_interface.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
    class Scanner;
}

namespace emu::lmc {

    class LmcOut : public InstructionInterface {
    public:
        LmcOut();

        Data eval() override;

        static std::unique_ptr<InstructionInterface> parse(Scanner &scanner);

    private:
        static const inline Data opcode = Data(902);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_OUT_H
