#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H

#include "chips/trivial/lmc/usings.h"
#include <sstream>
#include <vector>

namespace emu::lmc {

    class Assembler {
    public:
        static std::vector<Data> assemble(const std::stringstream &code);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
