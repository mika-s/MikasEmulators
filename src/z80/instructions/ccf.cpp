#include <iostream>
#include "doctest.h"
#include "z80/flags.h"

namespace emu::z80 {
    /**
     * Complement carry flag
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry</li>
     * </ul>
     *
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ccf(Flags &flag_reg, unsigned long &cycles) {
        flag_reg.toggle_carry_flag();

        cycles = 4;
    }

    void print_ccf(std::ostream &ostream) {
        ostream << "CCF";
    }

    TEST_CASE("Z80: CCF") {
        unsigned long cycles = 0;

        SUBCASE("should toggle the carry flag") {
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            ccf(flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            ccf(flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            Flags flag_reg;

            ccf(flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
