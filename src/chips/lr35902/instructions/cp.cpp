#include "chips/lr35902/flags.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::memory::NextByte;
using emu::util::byte::is_bit_set;
using emu::util::string::hexify_wo_0x;

void cp(u8& acc_reg, u8 value, Flags& flag_reg)
{
    u8 acc_reg_copy = acc_reg;

    sub_from_register(acc_reg_copy, value, false, flag_reg);
}

/**
 * Compare register with accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param value is the value to compare with the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void cp_r(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    cp(acc_reg, value, flag_reg);

    cycles = 4;
}

/**
 * Compare immediate with accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param args contains the argument with the immediate value
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void cp_n(u8& acc_reg, NextByte const& args, Flags& flag_reg, cyc& cycles)
{
    cp(acc_reg, args.farg, flag_reg);

    cycles = 7;
}

/**
 * Compare memory in HL's address with accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param value is the value to compare with the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void cp_MHL(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    cp(acc_reg, value, flag_reg);

    cycles = 7;
}

void print_cp(std::ostream& ostream, std::string const& reg)
{
    ostream << "CP "
            << reg;
}

void print_cp(std::ostream& ostream, NextByte const& args)
{
    ostream << "CP "
            << hexify_wo_0x(args.farg);
}

TEST_CASE("LR35902: CP")
{
    u8 acc_reg = 0;
    Flags flag_reg;

    SUBCASE("should compare the accumulator with value and set flags")
    {
        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (u8 value = 0; value < UINT8_MAX; ++value) {
                acc_reg = acc_reg_counter;

                cp(acc_reg, value, flag_reg);

                CHECK_EQ(static_cast<u8>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
            }
        }
    }
}

TEST_CASE("LR35902: CP r")
{
    cyc cycles = 0;
    u8 acc_reg = 0;

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;
        Flags flag_reg;

        cp_r(acc_reg, 0, flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}

TEST_CASE("LR35902: CP n")
{
    cyc cycles = 0;
    u8 acc_reg = 0;

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;
        NextByte args = { .farg = 0 };
        Flags flag_reg;

        cp_n(acc_reg, args, flag_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: CP [HL]")
{
    cyc cycles = 0;
    u8 acc_reg = 0;

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;
        Flags flag_reg;

        cp_MHL(acc_reg, 0, flag_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}
}
