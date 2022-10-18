#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::is_bit_set;

    /**
     * Set carry flag
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param flag_reg is the flag register, which will be mutated
     * @param acc_reg is the accumulator, and is used to set the X and Y flags
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void scf(Flags &flag_reg, u8 acc_reg, cyc &cycles) {
        flag_reg.set_carry_flag();
        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();

        if (is_bit_set(acc_reg, 5)) {
            flag_reg.set_y_flag();
        } else {
            flag_reg.clear_y_flag();
        }

        if (is_bit_set(acc_reg, 3)) {
            flag_reg.set_x_flag();
        } else {
            flag_reg.clear_x_flag();
        }

        cycles = 4;
    }

    void print_scf(std::ostream &ostream) {
        ostream << "SCF";
    }

    TEST_CASE("Z80: SCF") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should set the carry flag and always reset the half carry and add/subtract flags") {
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            scf(flag_reg, acc_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            flag_reg.set_half_carry_flag();
            flag_reg.set_add_subtract_flag();
            scf(flag_reg, acc_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            flag_reg.set_half_carry_flag();
            flag_reg.set_add_subtract_flag();
            scf(flag_reg, acc_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            Flags flag_reg;
            cycles = 0;

            scf(flag_reg, acc_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
