#include <iostream>
#include "doctest.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    void print_retn(std::ostream &ostream) {
        ostream << "RETN";
    }
}
