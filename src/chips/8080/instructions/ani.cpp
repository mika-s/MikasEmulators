#include "chips/8080/flags.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::i8080 {

using emu::memory::NextByte;
using emu::util::byte::is_bit_set;
using emu::util::string::hexify_wo_0x;

/**
 * And immediate with accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param args contains the argument with the immediate value
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ani(u8& acc_reg, NextByte const& args, Flags& flag_reg, cyc& cycles)
{
    const u8 previous = acc_reg;
    acc_reg &= args.farg;

    flag_reg.clear_carry_flag();
    flag_reg.handle_zero_flag(acc_reg);
    flag_reg.handle_sign_flag(acc_reg);
    flag_reg.handle_parity_flag(acc_reg);

    // The auxiliary carry is not supposed to be affected by ANI, according to Intel 8080 Assembly Language
    // Programming Manual (Rev B), but apparently has to be modified anyway. This is explained here:
    // https://demin.ws/blog/english/2012/12/24/my-i8080-collection/
    // The 8080/8085 Assembly Language Programming Manual also mentions auxiliary carry being modified by ANI.

    if (((previous | args.farg) & 0x08) != 0) {
        flag_reg.set_aux_carry_flag();
    } else {
        flag_reg.clear_aux_carry_flag();
    }

    cycles = 7;
}

void print_ani(std::ostream& ostream, NextByte const& args)
{
    ostream << "ANI "
            << hexify_wo_0x(args.farg);
}

TEST_CASE("8080: ANI")
{
    cyc cycles = 0;
    u8 acc_reg = 0;

    SUBCASE("should and the given value with the accumulator")
    {
        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (u8 value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;

                ani(acc_reg, args, flag_reg, cycles);

                CHECK_EQ(acc_reg_counter & value, acc_reg);
                CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                CHECK_EQ(false, flag_reg.is_carry_flag_set());
                CHECK_EQ(is_bit_set(acc_reg_counter | value, 3), flag_reg.is_aux_carry_flag_set());
            }
        }
    }

    SUBCASE("should set the parity flag when even parity")
    {
        Flags flag_reg;
        acc_reg = 0x3;
        NextByte args = { 0xff };

        ani(acc_reg, args, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_parity_flag_set());
    }

    SUBCASE("should not set the parity flag when odd parity")
    {
        Flags flag_reg;
        acc_reg = 0x2;
        NextByte args = { 0xff };

        ani(acc_reg, args, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_parity_flag_set());
    }

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;
        acc_reg = 0xe;
        NextByte args = { 0 };
        Flags flag_reg;

        ani(acc_reg, args, flag_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}
}
