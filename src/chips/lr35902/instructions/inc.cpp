#include "chips/lr35902/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
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

void inc(u8& reg, Flags& flag_reg)
{
    bool const old_carry = flag_reg.is_carry_flag_set();

    add_to_register(reg, 1, false, flag_reg);

    if (old_carry) {
        flag_reg.set_carry_flag();
    } else {
        flag_reg.clear_carry_flag();
    }
}

/**
 * Increment register
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to increment, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void inc_r(u8& reg, Flags& flag_reg, cyc& cycles)
{
    inc(reg, flag_reg);

    cycles = 4;
}

/**
 * Increment register (undocumented)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to increment, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void inc_r_undoc(u8& reg, Flags& flag_reg, cyc& cycles)
{
    inc(reg, flag_reg);

    cycles = 8;
}

/**
 * Increment the memory in HL's address
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 11</li>
 *   <li>Condition bits affected: half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param address is the address in HL
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void inc_MHL(EmulatorMemory<u16, u8>& memory, u16 address, Flags& flag_reg, cyc& cycles)
{
    u8 value = memory.read(address);
    inc(value, flag_reg);
    memory.write(address, value);

    cycles = 11;
}

/**
 * Increment register pair
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 6</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param reg1 is the first register in the register pair, which will be mutated
 * @param reg2 is the second register in the register pair, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void inc_ss(u8& reg1, u8& reg2, cyc& cycles)
{
    u16 val = to_u16(reg1, reg2);
    ++val;

    reg2 = low_byte(val);
    reg1 = high_byte(val);

    cycles = 6;
}

/**
 * Increment register pair (SP)
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 6</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void inc_sp(u16& sp, cyc& cycles)
{
    ++sp;

    cycles = 6;
}

void print_inc(std::ostream& ostream, std::string const& reg)
{
    ostream << "INC "
            << reg;
}

void print_inc_undocumented(std::ostream& ostream, std::string const& reg)
{
    ostream << "INC "
            << reg
            << "*";
}

TEST_CASE("LR35902: INC")
{
    SUBCASE("should increase register or memory")
    {
        u8 reg = 0;
        Flags flag_reg;

        inc(reg, flag_reg);

        CHECK_EQ(1, reg);

        inc(reg, flag_reg);

        CHECK_EQ(2, reg);

        inc(reg, flag_reg);

        CHECK_EQ(3, reg);

        inc(reg, flag_reg);

        CHECK_EQ(4, reg);

        inc(reg, flag_reg);

        CHECK_EQ(5, reg);
    }

    SUBCASE("should not affect the carry flag")
    {
        u8 reg = 255;
        Flags flag_reg;

        CHECK_EQ(false, flag_reg.is_carry_flag_set());

        inc(reg, flag_reg);

        CHECK_EQ(0, reg);
        CHECK_EQ(false, flag_reg.is_carry_flag_set());
    }

    SUBCASE("should set correct half carry flag")
    {
        u8 reg = 15;
        Flags flag_reg;

        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

        inc(reg, flag_reg);

        CHECK_EQ(16, reg);
        CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should set correct value in the zero flag")
    {
        u8 reg = 254;
        Flags flag_reg;

        inc(reg, flag_reg);

        CHECK_EQ(false, flag_reg.is_zero_flag_set());

        inc(reg, flag_reg);

        CHECK_EQ(true, flag_reg.is_zero_flag_set());
    }

    SUBCASE("should always unset the add/subtract flag")
    {
        u8 reg = 0;
        Flags flag_reg;
        flag_reg.set_add_subtract_flag();

        inc(reg, flag_reg);

        CHECK_EQ(1, reg);
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

        flag_reg.set_add_subtract_flag();

        inc(reg, flag_reg);

        CHECK_EQ(2, reg);
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

        flag_reg.set_add_subtract_flag();

        inc(reg, flag_reg);

        CHECK_EQ(3, reg);
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
    }
}

TEST_CASE("LR35902: INC r")
{
    SUBCASE("should use 4 cycles")
    {
        cyc cycles = 0;
        Flags flag_reg;
        u8 reg = UINT8_MAX;

        inc_r(reg, flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}

TEST_CASE("LR35902: INC ss")
{
    cyc cycles = 0;
    u8 reg1 = 0;
    u8 reg2 = 0;
    u8 expected_reg1 = 0;
    u8 expected_reg2;
    u16 sp = 0;

    SUBCASE("should increase register pair")
    {
        for (int i = 0; i < UINT16_MAX; ++i) {
            inc_ss(reg1, reg2, cycles);

            if (reg2 % (UINT8_MAX + 1) == 0 && i != 0) {
                ++expected_reg1;
            }

            expected_reg2 = i + 1;

            CHECK_EQ(expected_reg1, reg1);
            CHECK_EQ(expected_reg2, reg2);
        }
    }

    SUBCASE("should increase SP")
    {
        for (u16 expected_sp = 0; expected_sp < UINT16_MAX; ++expected_sp) {
            sp = expected_sp;

            inc_sp(sp, cycles);

            CHECK_EQ(expected_sp + 1, sp);
        }
    }

    SUBCASE("should use 6 cycles")
    {
        cycles = 0;

        inc_ss(reg1, reg2, cycles);

        CHECK_EQ(6, cycles);

        inc_sp(sp, cycles);

        CHECK_EQ(6, cycles);
    }
}

TEST_CASE("LR35902: INC (HL)")
{
    SUBCASE("should use 11 cycles")
    {
        cyc cycles = 0;
        Flags flag_reg;
        EmulatorMemory<u16, u8> memory;
        memory.add({ 0x01, 0x02 });

        inc_MHL(memory, 0x0001, flag_reg, cycles);

        CHECK_EQ(11, cycles);
    }
}
}
