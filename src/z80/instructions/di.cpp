#include <iostream>
#include "doctest.h"

namespace emu::z80 {
    /**
     * Disable interrupts
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
    void di(bool &iff1, bool &iff2, unsigned long &cycles) {
        iff1 = false;
        iff2 = false;

        cycles = 4;
    }

    void print_di(std::ostream &ostream) {
        ostream << "DI";
    }

    TEST_CASE("Z80: DI") {
        unsigned long cycles = 0;

        SUBCASE("should disable interrupts") {
            bool iff1 = true;
            bool iff2 = true;

            di(iff1, iff2, cycles);

            CHECK_EQ(false, iff1);
            CHECK_EQ(false, iff2);

            di(iff1, iff2, cycles);

            CHECK_EQ(false, iff1);
            CHECK_EQ(false, iff2);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            bool iff1 = true;
            bool iff2 = true;

            di(iff1, iff2, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
