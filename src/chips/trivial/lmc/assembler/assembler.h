#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H

#include "chips/trivial/lmc/usings.h"
#include <sstream>
#include <vector>

namespace emu::lmc {

    /**
     * A simple assembler that parses the file line by line.
     * It does not use a fancy parsing algorithm, just split etc.
     */
    class Assembler {
    public:
        static std::vector<Data> assemble(const std::stringstream &code);

    private:

    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_H
