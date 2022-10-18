#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {
    /**
     * Enable interrupts
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param iff1 is the main interrupt enable flip-flop, which will be mutated
     * @param iff2 is the alternate interrupt enable flip-flop, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ei(bool &iff1, bool &iff2, cyc &cycles) {
        iff1 = true;
        iff2 = true;

        cycles = 4;
    }

    void print_ei(std::ostream &ostream) {
        ostream << "EI";
    }

    TEST_CASE("Z80: EI") {
        cyc cycles = 0;

        SUBCASE("should enable interrupts") {
            bool iff1 = false;
            bool iff2 = false;

            ei(iff1, iff2, cycles);

            CHECK_EQ(true, iff1);
            CHECK_EQ(true, iff2);

            ei(iff1, iff2, cycles);

            CHECK_EQ(true, iff1);
            CHECK_EQ(true, iff2);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            bool iff1 = false;
            bool iff2 = false;

            ei(iff1, iff2, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
