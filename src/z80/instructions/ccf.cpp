#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::is_bit_set;

    /**
     * Complement carry flag
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
    void ccf(Flags &flag_reg, u8 acc_reg, unsigned long &cycles) {
        bool was_carry_set = flag_reg.is_carry_flag_set();
        flag_reg.toggle_carry_flag();
        flag_reg.clear_add_subtract_flag();
        if (was_carry_set) {
            flag_reg.set_half_carry_flag();
        } else {
            flag_reg.clear_half_carry_flag();
        }

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

    void print_ccf(std::ostream &ostream) {
        ostream << "CCF";
    }

    TEST_CASE("Z80: CCF") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should toggle the carry flag, set half carry flag to previous carry flag and always reset add/subtract flag") {
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            ccf(flag_reg, acc_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            ccf(flag_reg, acc_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            ccf(flag_reg, acc_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            Flags flag_reg;

            ccf(flag_reg, acc_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
