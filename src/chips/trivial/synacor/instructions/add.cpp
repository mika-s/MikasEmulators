#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::synacor {

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
void add()
{
}

void print_add(std::ostream& ostream)
{
    ostream << "ADD ";
}
}
