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
using emu::util::byte::unset_bit;

/**
 * Reset bit (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to reset
 * @param reg is the register to reset a bit in, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void res_r(unsigned int bit_number, u8& reg, cyc& cycles)
{
    assert(bit_number < 8);

    unset_bit(reg, bit_number);

    cycles = 8;
}

/**
 * Reset bit (value in memory location pointed to by HL)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 15</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param bit_number is the bit number to reset
 * @param hl_reg is the HL register pair
 * @param memory is the memory, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void res_MHL(unsigned int bit_number, u16 hl_reg, EmulatorMemory<u16, u8>& memory, cyc& cycles)
{
    assert(bit_number < 8);

    unset_bit_in_memory(memory, hl_reg, bit_number);

    cycles = 15;
}

void print_res(std::ostream& ostream, unsigned int bit_number, std::string const& src)
{
    assert(bit_number < 8);

    ostream << "RES "
            << bit_number
            << ", "
            << src;
}

TEST_CASE("LR35902: RES r")
{
    cyc cycles = 0;

    SUBCASE("should reset a bit in the registry")
    {
        for (u8 value_counter = 0; value_counter < UINT8_MAX; ++value_counter) {
            u8 expected = value_counter;
            u8 value = value_counter;

            for (unsigned int bit_number = 0; bit_number < 8; ++bit_number) {
                res_r(bit_number, value, cycles);

                unset_bit(expected, bit_number);

                CHECK_EQ(expected, value);
            }
        }
    }

    SUBCASE("should use 8 cycles")
    {
        cycles = 0;
        u8 value = 0;

        res_r(1, value, cycles);

        CHECK_EQ(8, cycles);
    }
}
}
