#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::synacor {

/**
 * Branch if zero
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param pc is the program counter, which will be mutated
 * @param address is the address to the value in memory
 */
void ret()
{
}

void print_ret(std::ostream& ostream)
{
    ostream << "RET ";
}
}
