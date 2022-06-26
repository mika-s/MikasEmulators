#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080 {
    /**
     * Complement accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cma(u8 &acc_reg, unsigned long &cycles) {
        acc_reg = ~acc_reg;

        cycles = 4;
    }

    void print_cma(std::ostream &ostream) {
        ostream << "CMA";
    }

    TEST_CASE("8080: CMA") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should complement the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cma(acc_reg, cycles);

                    CHECK_EQ(static_cast<u8>(~acc_reg_counter), acc_reg);
                }
            }
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            cma(acc_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
