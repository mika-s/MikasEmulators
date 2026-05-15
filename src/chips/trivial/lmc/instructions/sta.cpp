#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::lmc {

using emu::memory::EmulatorMemory;

/**
 * Store value in memory
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param address is the address to the value in memory
 * @param memory is the memory, which will be mutated
 */
void sta(const Data acc_reg, const Address address, EmulatorMemory<Address, Data>& memory)
{
    memory.write(address, acc_reg);
}

void print_sta(std::ostream& ostream, const Address address)
{
    ostream << "STA "
            << address;
}
}
