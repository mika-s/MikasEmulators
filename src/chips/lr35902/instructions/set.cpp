#include "chips/lr35902/util.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lr35902 {

using emu::memory::NextByte;
using emu::util::byte::set_bit;

/**
 * Set bit (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to set
 * @param reg is the register to set a bit in, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void set_r(unsigned int bit_number, u8& reg, cyc& cycles)
{
    assert(bit_number < 8);

    set_bit(reg, bit_number);

    cycles = 8;
}

/**
 * Set bit (value of memory location pointed to by HL)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 15</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to set
 * @param hl_reg is the HL register pair
 * @param memory is the memory, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void set_MHL(unsigned int bit_number, u16 hl_reg, EmulatorMemory<u16, u8>& memory, cyc& cycles)
{
    assert(bit_number < 8);

    set_bit_in_memory(memory, hl_reg, bit_number);

    cycles = 15;
}

/**
 * Set bit (value in memory pointed to by IX or IY plus d)
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 23</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to set
 * @param ixy_reg is the IX or IY register containing the base address
 * @param d contains address offset
 * @param memory is the memory, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void set_MixyPd(unsigned int bit_number, u16 ixy_reg, u8 d, EmulatorMemory<u16, u8>& memory, cyc& cycles)
{
    assert(bit_number < 8);

    set_bit_in_memory(memory, ixy_reg + static_cast<i8>(d), bit_number);

    cycles = 23;
}

void print_set(std::ostream& ostream, unsigned int bit_number, std::string const& src)
{
    assert(bit_number < 8);

    ostream << "SET "
            << bit_number
            << ", "
            << src;
}

void print_set_MixyPn(std::ostream& ostream, unsigned int bit_number, std::string const& ixy_reg, u8 d)
{
    assert(bit_number < 8);

    const i8 signed_value = static_cast<i8>(d);
    const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

    ostream << "SET "
            << bit_number
            << ", ("
            << ixy_reg
            << plus_or_minus
            << +signed_value
            << ")";
}

TEST_CASE("LR35902: SET r")
{
    cyc cycles = 0;

    SUBCASE("should set a bit in the registry")
    {
        for (u8 value_counter = 0; value_counter < UINT8_MAX; ++value_counter) {
            u8 expected = value_counter;
            u8 value = value_counter;

            for (unsigned int bit_number = 0; bit_number < 8; ++bit_number) {
                set_r(bit_number, value, cycles);

                set_bit(expected, bit_number);

                CHECK_EQ(expected, value);
            }
        }
    }

    SUBCASE("should use 8 cycles")
    {
        cycles = 0;
        u8 value = 0;

        set_r(1, value, cycles);

        CHECK_EQ(8, cycles);
    }
}
}
