#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::borrow_from;

    /**
     * Enable interrupts
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void neg(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        const u8 previous = acc_reg;
        acc_reg = 0 - acc_reg;

        if (previous != 0) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        if (previous == 0x80) {
            flag_reg.set_parity_overflow_flag();
        } else {
            flag_reg.clear_parity_overflow_flag();
        }

        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.set_add_subtract_flag();
        flag_reg.handle_half_borrow_flag(previous, acc_reg, false); // TODO: Fix

        cycles = 8;
    }

    void print_neg(std::ostream &ostream) {
        ostream << "NEG";
    }

    TEST_CASE("Z80: NEG") {
        unsigned long cycles = 0;

        SUBCASE("should negate the accumulator and set flags correctly") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                u8 acc_reg = acc_reg_counter;

                neg(acc_reg, flag_reg, cycles);

                CHECK_EQ(static_cast<u8>(0 - acc_reg_counter), acc_reg);
                CHECK_EQ(acc_reg_counter != 0, flag_reg.is_carry_flag_set());
                CHECK_EQ(acc_reg_counter == 0x80, flag_reg.is_parity_overflow_flag_set());
                CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
                // TODO: Test for half carry flag
//                CHECK_EQ(borrow_from(4, acc_reg_counter, acc_reg, false), flag_reg.is_half_carry_flag_set());
            }
        }

        SUBCASE("should use 8 cycles") {
            cycles = 0;
            u8 acc_reg = 0;
            Flags flag_reg;

            neg(acc_reg, flag_reg, cycles);

            CHECK_EQ(8, cycles);
        }
    }
}
