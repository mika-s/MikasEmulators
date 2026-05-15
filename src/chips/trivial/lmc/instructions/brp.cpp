#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "flags.h"
#include <iostream>

namespace emu::lmc {

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
void brp(Address& pc, const Address address, const Flags flag_reg) // NOLINT(*-identifier-length)
{
    if (!flag_reg.is_negative_flag_set()) {
        pc = address;
    }
}

void print_brp(std::ostream& ostream, const Address address)
{
    ostream << "BRP "
            << address;
}
}
