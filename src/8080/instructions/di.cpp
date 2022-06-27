#include <iostream>
#include "doctest.h"

namespace emu::i8080 {
    /**
     * Disable interrupts
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param inte is the interrupt bit, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void di(bool &inte, unsigned long &cycles) {
        inte = false;

        cycles = 4;
    }

    void print_di(std::ostream &ostream) {
        ostream << "DI";
    }

    TEST_CASE("8080: DI") {
        unsigned long cycles = 0;

        SUBCASE("should disable interrupts") {
            bool inte = true;

            di(inte, cycles);

            CHECK_EQ(false, inte);

            di(inte, cycles);

            CHECK_EQ(false, inte);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            bool inte = false;

            di(inte, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
