#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Stores 15-bit bitwise inverse of <b> in <a>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param a is the address of the register to store the not result into
 * @param b is the first operand to not
 * @param c is the second operand to not
 */
void not_(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b)
{
    memory.write(
        Address(a.underlying()),
        RawData(Data(~b.underlying()).underlying()));
}

void print_not(std::ostream& ostream, RawData a, RawData b)
{
    ostream << "ADD ";

    if (a >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(a.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(a.underlying()));
    }

    ostream << " ";

    if (b >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(b.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(b.underlying()));
    }
}
}
