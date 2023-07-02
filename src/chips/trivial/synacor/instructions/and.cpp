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
void and_()
{
}

void print_and(std::ostream& ostream)
{
    ostream << "AND ";
}
}
