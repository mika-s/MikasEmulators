#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H

#include "crosscutting/typedefs.h"
#include <sstream>
#include <vector>

namespace emu::lmc {

    class Assembler {
    public:
        static std::vector<u16> assemble(const std::stringstream &code);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
