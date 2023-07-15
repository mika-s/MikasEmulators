#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Store into <a> the product of <b> and <c> (modulo 32768)
 * <ul>
 *   <li>Size: 4</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param a is the address of the register to store the product into
 * @param b is the first operand to multiply
 * @param c is the second operand to multiply
 */
void mult(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c)
{
    memory.write(
        Address(a.underlying()),
        RawData(Data(b.underlying() * c.underlying()).underlying()));
}

void print_mult(std::ostream& ostream, RawData a, RawData b, RawData c)
{
    ostream << "MULT ";

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

    ostream << " ";

    if (c >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(c.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(c.underlying()));
    }
}
}
