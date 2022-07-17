#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {
    /**
     * Decimal adjust accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void daa(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        const u8 lower_nibble = acc_reg & 0x0f;
        const u8 upper_nibble = acc_reg >> 4;

        bool carry = flag_reg.is_carry_flag_set();
        u8 value_to_add = 0;

        if (lower_nibble > 9 || flag_reg.is_half_carry_flag_set()) {
            value_to_add = 6;
        }

        if (upper_nibble > 9 || carry || (lower_nibble > 9 && upper_nibble > 8)) {
            value_to_add += 6 << 4;
            carry = true;
        }

        add_to_register(acc_reg, value_to_add, false, flag_reg);

        if (carry) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        cycles = 4;
    }

    void print_daa(std::ostream &ostream) {
        ostream << "DAA";
    }

    TEST_CASE("Z80: DAA") {
        unsigned long cycles = 0;

        SUBCASE("should decimal adjust the accumulator") {
            // Example from the 8080 Programmers Manual.
            u8 acc_reg = 0x9b;
            Flags flag_reg;

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(0x01, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            u8 acc_reg = 0x9a;
            Flags flag_reg;

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_zero_flag_set());

            acc_reg = 0x9b;
            flag_reg.reset();

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            u8 acc_reg = 0x9a;
            Flags flag_reg;

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_sign_flag_set());
        }

//        SUBCASE("should set the parity flag when even parity") {
//            u8 acc_reg = 0x9a;
//            Flags flag_reg;
//
//            daa(acc_reg, flag_reg, cycles);
//
//            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
//        }
//
//        SUBCASE("should not set the parity flag when odd parity") {
//            u8 acc_reg = 0x9b;
//            Flags flag_reg;
//
//            daa(acc_reg, flag_reg, cycles);
//
//            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
//        }

        SUBCASE("should set the carry flag when carried out_Mn_A of msb") {
            u8 acc_reg = 0xff;
            Flags flag_reg;

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            u8 acc_reg = 0;
            Flags flag_reg;

            daa(acc_reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
