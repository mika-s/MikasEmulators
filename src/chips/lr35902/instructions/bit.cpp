#include "chips/lr35902/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void bit(unsigned int bit_number, u8 reg, Flags& flag_reg)
{
    assert(bit_number < 8);

    bool const is_set = is_bit_set(reg, bit_number);

    if (bit_number == msb && is_set) {
        flag_reg.set_sign_flag();
    } else {
        flag_reg.clear_sign_flag();
    }

    if (is_set) {
        flag_reg.clear_zero_flag();
        flag_reg.clear_parity_overflow_flag();
    } else {
        flag_reg.set_zero_flag();
        flag_reg.set_parity_overflow_flag();
    }

    flag_reg.set_half_carry_flag();
    flag_reg.clear_add_subtract_flag();
}

/**
 * Test bit (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param bit_number is the bit number to test
 * @param reg is the register to test a bit in
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void bit_r(unsigned int bit_number, u8 reg, Flags& flag_reg, cyc& cycles)
{
    assert(bit_number < 8);

    bit(bit_number, reg, flag_reg);

    if (is_bit_set(reg, 5)) {
        flag_reg.set_y_flag();
    } else {
        flag_reg.clear_y_flag();
    }

    if (is_bit_set(reg, 3)) {
        flag_reg.set_x_flag();
    } else {
        flag_reg.clear_x_flag();
    }

    cycles = 8;
}

/**
 * Test bit (value in memory pointed to by HL)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 12</li>
 *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param bit_number is the bit number to test
 * @param hl_reg is the HL register pair
 * @param memory is the memory
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void bit_MHL(unsigned int bit_number, u16 hl_reg, EmulatorMemory<u16, u8> const& memory, Flags& flag_reg, cyc& cycles)
{
    assert(bit_number < 8);

    bit(bit_number, memory.read(hl_reg), flag_reg);

    // TODO: Handle X and Y flags

    // TODO: memptr

    cycles = 12;
}

/**
 * Test bit (value in memory pointed to by IX or IY plus d)
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 20</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to reset
 * @param ixy_reg is the IX or IY register containing the base address
 * @param d contains address offset
 * @param memory is the memory
 * @param cycles is the number of cycles variable, which will be mutated
 */
void bit_MixyPd(unsigned int bit_number, u16 ixy_reg, u8 d, EmulatorMemory<u16, u8> const& memory, Flags& flag_reg, cyc& cycles)
{
    assert(bit_number < 8);

    bit(bit_number, memory.read(ixy_reg + static_cast<i8>(d)), flag_reg);

    // TODO: memptr

    cycles = 20;
}

void print_bit(std::ostream& ostream, unsigned int bit_number, std::string const& src)
{
    assert(bit_number < 8);

    ostream << "BIT "
            << bit_number
            << ", "
            << src;
}

void print_bit_MixyPn(std::ostream& ostream, unsigned int bit_number, std::string const& ixy_reg, u8 d)
{
    assert(bit_number < 8);

    const i8 signed_value = static_cast<i8>(d);
    const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

    ostream << "BIT "
            << bit_number
            << ", ("
            << ixy_reg
            << plus_or_minus
            << +signed_value
            << ")";
}

TEST_CASE("LR35902: BIT r")
{
    cyc cycles = 0;

    SUBCASE("should test a bit in the registry")
    {
        for (u8 value = 0; value < UINT8_MAX; ++value) {
            for (unsigned int bit_number = 0; bit_number < 8; ++bit_number) {
                Flags flag_reg;
                flag_reg.from_u8(0x00);

                bit_r(bit_number, value, flag_reg, cycles);

                if (bit_number == msb && is_bit_set(value, bit_number)) {
                    CHECK_EQ(true, flag_reg.is_sign_flag_set());
                } else {
                    CHECK_EQ(false, flag_reg.is_sign_flag_set());
                }

                if (is_bit_set(value, bit_number)) {
                    CHECK_EQ(false, flag_reg.is_zero_flag_set());
                    CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
                } else {
                    CHECK_EQ(true, flag_reg.is_zero_flag_set());
                    CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
                }

                CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
                CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

                if (is_bit_set(value, 5)) {
                    CHECK_EQ(true, flag_reg.is_y_flag_set());
                } else {
                    CHECK_EQ(false, flag_reg.is_y_flag_set());
                }

                if (is_bit_set(value, 3)) {
                    CHECK_EQ(true, flag_reg.is_x_flag_set());
                } else {
                    CHECK_EQ(false, flag_reg.is_x_flag_set());
                }
            }
        }
    }

    SUBCASE("should use 8 cycles")
    {
        cycles = 0;
        Flags flag_reg;
        flag_reg.from_u8(0x00);
        u8 value = 0;

        bit_r(1, value, flag_reg, cycles);

        CHECK_EQ(8, cycles);
    }
}
}
