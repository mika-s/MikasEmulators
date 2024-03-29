#include "chips/lr35902/flags.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::util::string::hexify_wo_0x;

void or_(u8& acc_reg, u8 value, Flags& flag_reg)
{
    acc_reg |= value;

    flag_reg.clear_carry_flag();
    flag_reg.clear_half_carry_flag();
    flag_reg.handle_zero_flag(acc_reg);
    flag_reg.clear_add_subtract_flag();
}

/**
 * Inclusive or with accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param value contains the argument that should be ored with the accumulator
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void or_r(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    or_(acc_reg, value, flag_reg);

    cycles = 4;
}

/**
 * Or immediate with accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param args contains the argument with the immediate value
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void or_n(u8& acc_reg, NextByte const& args, Flags& flag_reg, cyc& cycles)
{
    or_(acc_reg, args.farg, flag_reg);

    cycles = 7;
}

/**
 * Inclusive or memory in HL's address with accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param value contains the argument that should be ored with the accumulator
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void or_MHL(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    or_(acc_reg, value, flag_reg);

    cycles = 7;
}

void print_or_r(std::ostream& ostream, std::string const& reg)
{
    ostream << "OR "
            << reg;
}

void print_or_n(std::ostream& ostream, NextByte const& args)
{
    ostream << "OR "
            << hexify_wo_0x(args.farg);
}

TEST_CASE("LR35902: OR")
{
    u8 acc_reg = 0;

    SUBCASE("should or the given value with the accumulator")
    {
        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (u8 value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;

                or_(acc_reg, value, flag_reg);

                CHECK_EQ(acc_reg_counter | value, acc_reg);
                CHECK_EQ(false, flag_reg.is_carry_flag_set());
                CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
                CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
            }
        }
    }
}

TEST_CASE("LR35902: OR r")
{
    SUBCASE("should use 4 cycles")
    {
        cyc cycles = 0;
        u8 acc_reg = 0xe;
        u8 value = 0;
        Flags flag_reg;

        or_r(acc_reg, value, flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}

TEST_CASE("LR35902: OR n")
{
    SUBCASE("should use 7 cycles")
    {
        cyc cycles = 0;
        u8 acc_reg = 0xe;
        NextByte args = { 0 };
        Flags flag_reg;

        or_n(acc_reg, args, flag_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: OR [HL]")
{
    SUBCASE("should use 7 cycles")
    {
        cyc cycles = 0;
        u8 acc_reg = 0xe;
        u8 value = 0;
        Flags flag_reg;

        or_MHL(acc_reg, value, flag_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}
}
