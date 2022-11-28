#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_PROGRAM_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_PROGRAM_H

#include "environment.h"
#include "lmc_line.h"
#include "usings.h"
#include <vector>

namespace emu::lmc {
    class Scanner;
}

namespace emu::lmc {
    class LmcProgram {
    public:
        explicit LmcProgram(std::vector<LmcLine> lines);

        std::vector<Data> eval();

        static LmcProgram parse(Scanner &scanner, Environment &environment);

    private:
        std::vector<LmcLine> m_lines;
        Environment m_environment;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_LMC_PROGRAM_H
