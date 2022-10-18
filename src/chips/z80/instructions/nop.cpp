#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {
    /**
     * No operation
     * <ul>
     *   <li>Size: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void nop(cyc &cycles) {
        cycles = 4;
    }

    void print_nop(std::ostream &ostream) {
        ostream << "NOP";
    }

    TEST_CASE("Z80: NOP") {
        cyc cycles = 0;

        SUBCASE("should use 4 cycles") {
            nop(cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
