#include "chips/lr35902/flags.h"
#include "crosscutting/memory/emulator_memory.h"
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
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

void add(u8& acc_reg, u8 value, Flags& flag_reg)
{
    add_to_register(acc_reg, value, false, flag_reg);
}

void add(u16& reg, u16 value, Flags& flag_reg)
{
    bool was_sign_flag_set = flag_reg.is_sign_flag_set();
    bool was_zero_flag_set = flag_reg.is_zero_flag_set();
    bool was_parity_overflow_flag_set = flag_reg.is_parity_overflow_flag_set();

    add_to_register(reg, value, false, flag_reg);

    if (was_sign_flag_set) {
        flag_reg.set_sign_flag();
    } else {
        flag_reg.clear_sign_flag();
    }

    if (was_zero_flag_set) {
        flag_reg.set_zero_flag();
    } else {
        flag_reg.clear_zero_flag();
    }

    if (was_parity_overflow_flag_set) {
        flag_reg.set_parity_overflow_flag();
    } else {
        flag_reg.clear_parity_overflow_flag();
    }
}

/**
 * Add from register to accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param value is the value to add to the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_r(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    add(acc_reg, value, flag_reg);

    cycles = 4;
}

/**
 * Add from IX or IY high or low to accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param ixy_reg_h_or_l is the value to add to the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_ixy_h_or_l(u8& acc_reg, u8 ixy_reg_h_or_l, Flags& flag_reg, cyc& cycles)
{
    add(acc_reg, ixy_reg_h_or_l, flag_reg);

    cycles = 8;
}

/**
 * Add value pointed to by IX or IY plus d to accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 19</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param ixy_reg is the IX or IY register containing the base address
 * @param args contains address offset
 * @param memory is the memory, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_MixyPd(u8& acc_reg, u16 ixy_reg, NextByte const& args, EmulatorMemory<u16, u8>& memory, Flags& flag_reg,
    cyc& cycles)
{
    const u16 address = ixy_reg + static_cast<i8>(args.farg);
    u8 value = memory.read(address);

    add(acc_reg, value, flag_reg);

    memory.write(address, value);

    cycles = 19;
}

/**
 * Add immediate to accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param args contains the argument with the immediate value
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_n(u8& acc_reg, NextByte const& args, Flags& flag_reg, cyc& cycles)
{
    add(acc_reg, args.farg, flag_reg);

    cycles = 7;
}

/**
 * Add memory in HL's address to accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param value is the value to add to the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_MHL(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    add(acc_reg, value, flag_reg);

    cycles = 7;
}

/**
 * Double register add
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 11</li>
 *   <li>Condition bits affected: carry, half carry, add/subtract</li>
 * </ul>
 *
 * @param h_reg is the H register, which will be mutated
 * @param l_reg is the L register, which will be mutated
 * @param value contains the argument that should be added to HL
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_HL_ss(u8& h_reg, u8& l_reg, u16 value, Flags& flag_reg, cyc& cycles)
{
    u16 hl = to_u16(h_reg, l_reg);

    add(hl, value, flag_reg);

    h_reg = high_byte(hl);
    l_reg = low_byte(hl);

    cycles = 11;
}

/**
 * Double register add to IX or IY
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 15</li>
 *   <li>Condition bits affected: carry, half carry, add/subtract</li>
 * </ul>
 *
 * @param ixy_reg is the IX or IY register, which will be mutated
 * @param value_to_add contains the argument that should be added to IX or IY
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_ixy_pp(u16& ixy_reg, u16 value_to_add, Flags& flag_reg, cyc& cycles)
{
    add(ixy_reg, value_to_add, flag_reg);

    cycles = 15;
}

/************************************ FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS *************************************/

/**
 * Add from register to accumulator (undocumented)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param value is the value to add to the accumulator register
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void add_A_r_undoc(u8& acc_reg, u8 value, Flags& flag_reg, cyc& cycles)
{
    add(acc_reg, value, flag_reg);

    cycles = 8;
}

/******************************** END OF FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS **********************************/

void print_add(std::ostream& ostream, std::string const& dest, std::string const& src)
{
    ostream << "ADD "
            << dest
            << ", "
            << src;
}

void print_add_undocumented(std::ostream& ostream, std::string const& dest, std::string const& src)
{
    ostream << "ADD "
            << dest
            << ", "
            << src
            << "*";
}

void print_add_r_n(std::ostream& ostream, std::string const& reg, NextByte const& args)
{
    ostream << "ADD "
            << reg
            << ", "
            << hexify_wo_0x(args.farg);
}

void print_add_MixyPn(std::ostream& ostream, std::string const& reg, std::string const& ixy_reg,
    NextByte const& args)
{
    const i8 signed_value = static_cast<i8>(args.farg);
    const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

    ostream << "ADD "
            << reg
            << ",("
            << ixy_reg
            << plus_or_minus
            << +signed_value
            << ")";
}

TEST_CASE("LR35902: ADD")
{
    u8 acc_reg = 0;

    SUBCASE("should add given value to the accumulator")
    {
        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (u8 value = 0; value < UINT8_MAX; ++value) {
                for (int carry = 0; carry < 2; ++carry) {
                    Flags flag_reg;
                    if (carry) {
                        flag_reg.set_carry_flag();
                    } else {
                        flag_reg.clear_carry_flag();
                    }

                    acc_reg = acc_reg_counter;

                    add(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter + value), acc_reg);
                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(static_cast<i8>(acc_reg) < 0, flag_reg.is_sign_flag_set());
                    CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
                    CHECK_EQ(
                        (((acc_reg_counter & 0xf) + (value & 0xf)) & 0x10) > 0,
                        flag_reg.is_half_carry_flag_set());

                    bool const are_same_sign = (acc_reg_counter <= INT8_MAX && value <= INT8_MAX)
                        || (INT8_MAX < acc_reg_counter && INT8_MAX < value);
                    bool const are_positive = are_same_sign && acc_reg_counter <= INT8_MAX;
                    bool const are_negative = are_same_sign && INT8_MAX < acc_reg_counter;
                    bool const goes_from_positive_to_negative = are_positive && acc_reg > INT8_MAX;
                    bool const goes_negative_to_positive = are_negative && acc_reg <= INT8_MAX;
                    CHECK_EQ(
                        goes_from_positive_to_negative || goes_negative_to_positive,
                        flag_reg.is_parity_overflow_flag_set());

                    // TODO: Move carry here
                }
            }
        }
    }

    SUBCASE("should set the carry flag when carried out of msb")
    {
        Flags flag_reg;
        acc_reg = 0x80;

        add(acc_reg, 0x80, flag_reg);

        CHECK_EQ(0x0, acc_reg);
        CHECK_EQ(true, flag_reg.is_carry_flag_set());
    }

    SUBCASE("should not set the carry flag when not carried out of msb")
    {
        Flags flag_reg;
        acc_reg = 0x1;

        add(acc_reg, 0x2, flag_reg);

        CHECK_EQ(0x3, acc_reg);
        CHECK_EQ(false, flag_reg.is_carry_flag_set());
    }
}

TEST_CASE("LR35902: ADD A, r")
{
    SUBCASE("should use 4 cycles")
    {
        cyc cycles = 0;
        Flags flag_reg;
        u8 acc_reg = 0xe;

        add_A_r(acc_reg, 0x1, flag_reg, cycles);
        CHECK_EQ(4, cycles);
    }
}

TEST_CASE("LR35902: ADD A, n")
{
    SUBCASE("should use 7 cycles")
    {
        cyc cycles = 0;
        Flags flag_reg;
        u8 acc_reg = 0xe;
        NextByte args = { 0x1 };

        add_A_n(acc_reg, args, flag_reg, cycles);
        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: ADD A, [HL]")
{
    SUBCASE("should use 7 cycles")
    {
        cyc cycles = 0;
        Flags flag_reg;
        u8 acc_reg = 0xe;

        add_A_MHL(acc_reg, 0x1, flag_reg, cycles);
        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: ADD HL, ss")
{
    cyc cycles = 0;

    SUBCASE("should add the given value to HL")
    {
        for (u16 hl_counter = 0; hl_counter < 100; ++hl_counter) {
            for (u16 value_to_add = 0; value_to_add < UINT16_MAX; ++value_to_add) {
                Flags flag_reg;

                u8 h_reg = high_byte(hl_counter);
                u8 l_reg = low_byte(hl_counter);

                add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

                CHECK_EQ(
                    static_cast<u16>(hl_counter + value_to_add),
                    to_u16(h_reg, l_reg));

                CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
            }
        }
    }

    SUBCASE("should set the carry flag when carried out of msb")
    {
        Flags flag_reg;

        u8 h_reg = 0xff;
        u8 l_reg = 0xff;
        u16 value_to_add = 0x1;

        add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
    }

    //        SUBCASE("should set the half carry flag when carried out of the 11th bit") { // TODO: Figure out half carry for dad
    //            Flags flag_reg;
    //
    //            u8 h_reg = 0x08;
    //            u8 l_reg = 0x00;
    //            u16 value_to_add = 0x1;
    //
    //            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    //
    //            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);
    //
    //            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
    //        }

    SUBCASE("should use 11 cycles")
    {
        cycles = 0;
        Flags flag_reg;

        u8 h_reg = 0;
        u8 l_reg = 0xE;
        u16 value_to_add = 0x4;

        add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

        CHECK_EQ(11, cycles);
    }
}
}
