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
     * @param iff is the interrupt enable flip-flop, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void di(bool &iff, unsigned long &cycles) {
        iff = false;

        cycles = 4;
    }

    void print_di(std::ostream &ostream) {
        ostream << "DI";
    }

    TEST_CASE("Z80: DI") {
        unsigned long cycles = 0;

        SUBCASE("should disable interrupts") {
            bool iff = true;

            di(iff, cycles);

            CHECK_EQ(false, iff);

            di(iff, cycles);

            CHECK_EQ(false, iff);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            bool iff = false;

            di(iff, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
