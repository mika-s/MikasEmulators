#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::synacor {

/**
 * Branch if positive
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param address is the address to the value in memory
 * @param flag_reg is the flag register
 */
void out()
{
}

void print_out(std::ostream& ostream)
{
    ostream << "OUT ";
}
}
