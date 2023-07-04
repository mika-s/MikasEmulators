#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::util::string::hexify;

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
void mult()
{
}

void print_mult(std::ostream& ostream, Address a, Data b, Data c)
{
    ostream << "MULT "
            << hexify(static_cast<u16>(a.underlying()))
            << " "
            << hexify(static_cast<u16>(b.underlying()))
            << " "
            << hexify(static_cast<u16>(c.underlying()));
}
}
