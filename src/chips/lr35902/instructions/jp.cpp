#include "chips/lr35902/flags.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::memory::NextWord;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

/**
 * Jump
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the address to jump to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp(u16& pc, NextWord const& args, cyc& cycles)
{
    pc = to_u16(args.sarg, args.farg);

    cycles = 10;
}

/**
 * Jump if not zero
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the address to jump to
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp_nz(u16& pc, NextWord const& args, Flags const& flag_reg, cyc& cycles)
{
    if (!flag_reg.is_zero_flag_set()) {
        pc = to_u16(args.sarg, args.farg);
    }

    cycles = 10;
}

/**
 * Jump if zero
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the address to jump to
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp_z(u16& pc, NextWord const& args, Flags const& flag_reg, cyc& cycles)
{
    if (flag_reg.is_zero_flag_set()) {
        pc = to_u16(args.sarg, args.farg);
    }

    cycles = 10;
}

/**
 * Jump if no carry
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the address to jump to
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp_nc(u16& pc, NextWord const& args, Flags const& flag_reg, cyc& cycles)
{
    if (!flag_reg.is_carry_flag_set()) {
        pc = to_u16(args.sarg, args.farg);
    }

    cycles = 10;
}

/**
 * Jump if carry
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 2 or 3</li>
 *   <li>States: 7 or 12</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the address to jump to
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp_c(u16& pc, NextWord const& args, Flags const& flag_reg, cyc& cycles)
{
    if (flag_reg.is_carry_flag_set()) {
        pc = to_u16(args.sarg, args.farg);
    }

    cycles = 10;
}

/**
 * Jump to address in HL
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param address is the address in the HL register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void jp_hl(u16& pc, u16 address, cyc& cycles)
{
    pc = address;

    cycles = 4;
}

void print_jp_nn(std::ostream& ostream, NextWord const& args)
{
    ostream << "JP "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

void print_jp_cc_nn(std::ostream& ostream, NextWord const& args, std::string const& condition)
{
    ostream << "JP "
            << condition
            << ","
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

void print_jp_Mss(std::ostream& ostream, std::string const& reg)
{
    ostream << "JP "
            << "("
            << reg
            << ")";
}

TEST_CASE("LR35902: JP")
{
    cyc cycles = 0;
    u16 pc = 0;
    NextWord args = { .farg = 0x11, .sarg = 0x22 };

    SUBCASE("should jump")
    {
        jp(pc, args, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;

        jp(pc, args, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: JP NZ")
{
    cyc cycles = 0;

    SUBCASE("should jump when zero flag is set")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jp_nz(pc, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
    }

    SUBCASE("should not jump when zero flag is set")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jp_nz(pc, args, flag_reg, cycles);

        CHECK_EQ(0, pc);
    }

    SUBCASE("should not affect any flags")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jp_nz(pc, args, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_zero_flag_set());
        CHECK_EQ(false, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jp_nz(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);

        flag_reg.clear_zero_flag();

        jp_nz(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: JP Z")
{
    cyc cycles = 0;

    SUBCASE("should jump when the zero flag is set")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jp_z(pc, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
    }

    SUBCASE("should not jump when the zero flag is unset")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jp_z(pc, args, flag_reg, cycles);

        CHECK_EQ(0, pc);
    }

    SUBCASE("should not affect any flags")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jp_z(pc, args, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_zero_flag_set());
        CHECK_EQ(false, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jp_z(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);

        flag_reg.set_zero_flag();

        jp_z(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: JP NC")
{
    cyc cycles = 0;

    SUBCASE("should jump when the carry flag is unset")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        jp_nc(pc, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
    }

    SUBCASE("should not jump when the carry flag is set")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        jp_nc(pc, args, flag_reg, cycles);

        CHECK_EQ(0, pc);
    }

    SUBCASE("should not affect any flags")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        jp_nc(pc, args, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_zero_flag_set());
        CHECK_EQ(false, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        jp_nc(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);

        flag_reg.clear_carry_flag();

        jp_nc(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: JP C")
{
    cyc cycles = 0;

    SUBCASE("should jump when the carry flag is set")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        jp_c(pc, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
    }

    SUBCASE("should not jump when the carry flag is unset")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        jp_c(pc, args, flag_reg, cycles);

        CHECK_EQ(0, pc);
    }

    SUBCASE("should not affect any flags")
    {
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        jp_c(pc, args, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_zero_flag_set());
        CHECK_EQ(true, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;
        u16 pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        jp_c(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);

        flag_reg.set_carry_flag();

        jp_c(pc, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: JP (HL)")
{
    cyc cycles = 0;
    u16 pc = 0x1111;
    u16 address = 0x432a;

    SUBCASE("should load address into PC")
    {
        jp_hl(pc, address, cycles);

        CHECK_EQ(address, pc);
    }

    SUBCASE("should use 4 cycles")
    {
        cycles = 0;

        jp_hl(pc, address, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
