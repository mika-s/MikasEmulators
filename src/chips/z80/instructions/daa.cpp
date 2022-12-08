#include "chips/z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>

namespace emu::z80 {

using emu::util::byte::is_bit_set;

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
void daa(u8& acc_reg, Flags& flag_reg, cyc& cycles)
{
    const u8 lower_nibble = acc_reg & 0x0f;
    const u8 upper_nibble = acc_reg >> 4;

    bool carry = flag_reg.is_carry_flag_set();
    u8 value_to_add_or_subtract = 0;

    if (lower_nibble > 9 || flag_reg.is_half_carry_flag_set()) {
        value_to_add_or_subtract = 6;
    }

    if (upper_nibble > 9 || carry || (lower_nibble > 9 && upper_nibble > 8)) {
        value_to_add_or_subtract += 6 << 4;
        carry = true;
    }

    bool const add_subtract_flag = flag_reg.is_add_subtract_flag_set();

    if (add_subtract_flag) {
        sub_from_register(acc_reg, value_to_add_or_subtract, false, flag_reg);
    } else {
        add_to_register(acc_reg, value_to_add_or_subtract, false, flag_reg);
    }

    flag_reg.handle_parity_flag(acc_reg);

    if (carry) {
        flag_reg.set_carry_flag();
    } else {
        flag_reg.clear_carry_flag();
    }

    if (add_subtract_flag) {
        flag_reg.set_add_subtract_flag();
    } else {
        flag_reg.clear_add_subtract_flag();
    }

    cycles = 4;
}

void print_daa(std::ostream& ostream)
{
    ostream << "DAA";
}

TEST_CASE("Z80: DAA")
{
    cyc cycles = 0;

    SUBCASE("should decimal adjust the accumulator")
    {
        // Example from the 8080 Programmers Manual.
        u8 acc_reg = 0x9b;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);

        CHECK_EQ(0x01, acc_reg);
        CHECK_EQ(true, flag_reg.is_carry_flag_set());
        CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should set the zero flag when zero and not set otherwise")
    {
        u8 acc_reg = 0x9a;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_zero_flag_set());

        acc_reg = 0x9b;
        flag_reg.reset();

        daa(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_zero_flag_set());
    }

    SUBCASE("should set the sign flag when above 127 and not otherwise")
    {
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

    SUBCASE("should set the carry flag when carried out of msb")
    {
        u8 acc_reg = 0xff;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
    }

    SUBCASE("should use 4 cycles")
    {
        u8 acc_reg = 0;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
