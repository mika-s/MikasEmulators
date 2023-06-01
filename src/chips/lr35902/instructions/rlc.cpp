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
using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void rlc(u8& value, Flags& flag_reg)
{
    bool const should_set_carry_flag = is_bit_set(value, msb);
    value = value << 1;
    if (should_set_carry_flag) {
        flag_reg.set_carry_flag();
        set_bit(value, lsb);
    } else {
        flag_reg.clear_carry_flag();
    }

    flag_reg.clear_half_carry_flag();
    flag_reg.clear_add_subtract_flag();
}

/**
 * Rotate left circular (accumulator)
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void rlca(u8& acc_reg, Flags& flag_reg, cyc& cycles)
{
    rlc(acc_reg, flag_reg);
    flag_reg.clear_zero_flag();

    cycles = 4;
}

/**
 * Rotate left circular (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to rotate, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void rlc_r(u8& reg, Flags& flag_reg, cyc& cycles)
{
    rlc(reg, flag_reg);

    flag_reg.handle_zero_flag(reg);

    cycles = 8;
}

/**
 * Rotate left circular (value in memory at HL's address)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param address is the address in HL
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void rlc_MHL(EmulatorMemory<u16, u8>& memory, u16 address, Flags& flag_reg, cyc& cycles)
{
    u8 value = memory.read(address);

    rlc(value, flag_reg);

    memory.write(address, value);

    flag_reg.handle_zero_flag(value);

    cycles = 16;
}

void print_rlca(std::ostream& ostream)
{
    ostream << "RLCA";
}

void print_rlc(std::ostream& ostream, std::string const& reg)
{
    ostream << "RLC "
            << reg;
}

TEST_CASE("LR35902: RLCA")
{
    cyc cycles = 0;
    u8 acc_reg = 0;

    SUBCASE("should rotate the accumulator left")
    {
        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            Flags flag_reg;
            acc_reg = acc_reg_counter;
            bool const cy = is_bit_set(acc_reg, msb);

            rlca(acc_reg, flag_reg, cycles);

            u8 expected = acc_reg_counter << 1u;
            if (cy) {
                set_bit(expected, lsb);
            }

            CHECK_EQ(expected, acc_reg);
        }
    }

    SUBCASE("should set the carry flag if shifted out of msb")
    {
        acc_reg = 0b10000000;
        Flags flag_reg;

        CHECK_EQ(false, flag_reg.is_carry_flag_set());

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
    }

    SUBCASE("should not take the carry flag into account when shifting")
    {
        acc_reg = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(0, acc_reg);
    }

    SUBCASE("should always reset the half carry flag")
    {
        acc_reg = 0;
        Flags flag_reg;
        flag_reg.set_half_carry_flag();

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
    }

    SUBCASE("should always reset the add/subtract flag")
    {
        acc_reg = 0;
        Flags flag_reg;
        flag_reg.set_add_subtract_flag();

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
    }

    SUBCASE("should use 4 cycles")
    {
        cycles = 0;
        acc_reg = 1;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        rlca(acc_reg, flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
