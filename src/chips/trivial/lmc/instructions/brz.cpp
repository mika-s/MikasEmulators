#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::lmc {

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
void brz(const Data acc_reg, Address& pc, const Address address) // NOLINT(*-identifier-length)
{
    if (acc_reg.underlying() == 0) {
        pc = address;
    }
}

void print_brz(std::ostream& ostream, const Address address)
{
    ostream << "BRZ "
            << address;
}
}
