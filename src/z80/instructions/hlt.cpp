#include <iostream>
#include "doctest.h"

namespace emu::z80 {
    /**
     * Halt
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param stopped is the stopped status variable
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void hlt(bool &stopped, unsigned long &cycles) {
        stopped = true;

        cycles = 7;
    }

    void print_hlt(std::ostream &ostream) {
        ostream << "HLT";
    }

    TEST_CASE("Z80: HLT") {
        unsigned long cycles = 0;

        SUBCASE("should stop the program") {
            bool stopped = false;

            hlt(stopped, cycles);

            CHECK_EQ(true, stopped);

            hlt(stopped, cycles);

            CHECK_EQ(true, stopped);
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;

            bool stopped = false;

            hlt(stopped, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
