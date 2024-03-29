#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {
class Flags;
}

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::util::byte::high_nibble;
using emu::util::byte::low_nibble;

/**
 * Swap low and high nibble of a register
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to swap nibbles, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void swap(u8& reg, [[maybe_unused]] Flags& flag_reg, cyc& cycles)
{
    u8 const high = high_nibble(reg);
    u8 const low = low_nibble(reg);

    reg = (low << 4) | high;

    cycles = 8;
}

/**
 * Swap low and high nibble of a register
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param address is the address in HL
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void swap_MHL(EmulatorMemory<u16, u8>& memory, u16 address, [[maybe_unused]] Flags& flag_reg, cyc& cycles)
{
    u8 value = memory.read(address);

    u8 const high = high_nibble(value);
    u8 const low = low_nibble(value);

    value = (low << 4) | high;

    memory.write(address, value);

    cycles = 16;
}

void print_swap(std::ostream& ostream, std::string const& reg)
{
    ostream << "SWAP "
            << reg;
}
}
