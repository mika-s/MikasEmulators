#include <iostream>
#include "doctest.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    void print_cpi(std::ostream &ostream) {
        ostream << "CPI";
    }
}
