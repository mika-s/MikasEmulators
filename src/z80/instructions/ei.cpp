#include <iostream>
#include "doctest.h"

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
     * @param iff is the interrupt enable flip-flop, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ei(bool &iff, unsigned long &cycles) {
        iff = true;

        cycles = 4;
    }

    void print_ei(std::ostream &ostream) {
        ostream << "EI";
    }

    TEST_CASE("Z80: EI") {
        unsigned long cycles = 0;

        SUBCASE("should enable interrupts") {
            bool iff = false;

            ei(iff, cycles);

            CHECK_EQ(true, iff);

            ei(iff, cycles);

            CHECK_EQ(true, iff);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            bool iff = false;

            ei(iff, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
