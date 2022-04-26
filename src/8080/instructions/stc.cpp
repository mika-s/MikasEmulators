#include <iostream>
#include "doctest.h"
#include "8080/flags.h"

namespace emu::cpu8080 {
    /**
     * Set carry
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
    void stc(Flags &flag_reg, unsigned long &cycles) {
        flag_reg.set_carry_flag();

        cycles = 4;
    }

    void print_stc(std::ostream &ostream) {
        ostream << "STC";
    }

    TEST_CASE("8080: STC") {
        unsigned long cycles = 0;
        Flags flag_reg;

        SUBCASE("should set the carry flag") {
            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            stc(flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            stc(flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
