#include <iostream>
#include "doctest.h"

namespace emu::z80 {
    /**
     * Halt
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param stopped is the stopped status variable
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void halt(bool &stopped, unsigned long &cycles) {
        stopped = true;

        cycles = 4;
    }

    void print_halt(std::ostream &ostream) {
        ostream << "HALT";
    }

    TEST_CASE("Z80: HALT") {
        unsigned long cycles = 0;

        SUBCASE("should stop the program") {
            bool stopped = false;

            halt(stopped, cycles);

            CHECK_EQ(true, stopped);

            halt(stopped, cycles);

            CHECK_EQ(true, stopped);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            bool stopped = false;

            halt(stopped, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
