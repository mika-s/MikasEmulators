#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::i8080 {

using emu::memory::EmulatorMemory;
using emu::util::byte::to_u16;

/**
 * Load accumulator indirect
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param reg1 contains the MSB of the address to load
 * @param reg2 contains the LSB of the address to load
 * @param memory is the memory
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ldax(u8& acc_reg, u8 reg1, u8 reg2, EmulatorMemory<u16, u8> const& memory, cyc& cycles)
{
    acc_reg = memory.read(to_u16(reg1, reg2));

    cycles = 7;
}

void print_ldax(std::ostream& ostream, std::string const& reg)
{
    ostream << "LDAX "
            << reg;
}

TEST_CASE("8080: LDAX")
{
    cyc cycles = 0;
    u8 acc_reg = 0xe;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    u8 reg1 = 0;
    u8 reg2 = 0x04;

    SUBCASE("should load the accumulator from memory using address in args")
    {
        ldax(acc_reg, reg1, reg2, memory, cycles);

        CHECK_EQ(memory.read(0x04), acc_reg);
    }

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;

        ldax(acc_reg, reg1, reg2, memory, cycles);

        CHECK_EQ(7, cycles);
    }
}
}
