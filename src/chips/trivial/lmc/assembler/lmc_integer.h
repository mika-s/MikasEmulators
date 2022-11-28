#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INTEGER_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INTEGER_H

#include "usings.h"

namespace emu::lmc {
    class Scanner;
}

namespace emu::lmc {

    class LmcInteger {
    public:
        explicit LmcInteger(int literal);

        Data eval();

        static LmcInteger parse(Scanner &scanner);

    private:
        [[maybe_unused]] int m_literal;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_INTEGER_H
