#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

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
void wmem()
{
}

void print_wmem(std::ostream& ostream, Address a, Address b)
{
    ostream << "WMEM "
            << hexify(static_cast<u16>(a.underlying()))
            << " "
            << hexify(static_cast<u16>(b.underlying()));
}
}
