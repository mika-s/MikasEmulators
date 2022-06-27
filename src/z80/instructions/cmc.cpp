#include <iostream>
#include "doctest.h"
#include "z80/flags.h"

namespace emu::z80 {
    /**
     * Complement carry
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
    void cmc(Flags &flag_reg, unsigned long &cycles) {
        flag_reg.toggle_carry_flag();

        cycles = 4;
    }

    void print_cmc(std::ostream &ostream) {
        ostream << "CMC";
    }

    TEST_CASE("Z80: CMC") {
        unsigned long cycles = 0;

        SUBCASE("should toggle the carry flag") {
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            cmc(flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            cmc(flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            Flags flag_reg;

            cmc(flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
