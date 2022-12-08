#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::lmc {

using emu::memory::EmulatorMemory;

/**
 * Add value in memory to the accumulator
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param address is the address to the value in memory
 * @param memory is the memory
 */
void add(Data& acc_reg, Address address, EmulatorMemory<Address, Data> const& memory)
{
    acc_reg += memory.read(address);
}

void print_add(std::ostream& ostream, Address address)
{
    ostream << "ADD "
            << address;
}
}
