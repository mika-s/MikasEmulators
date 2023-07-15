#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
 * <ul>
 *   <li>Size: 4</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param a is the value set to 0 or 1
 * @param b is the first operand to check for equality
 * @param c is the second operand to check for equality
 */
void eq(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c)
{
    if (b == c) {
        memory.write(Address(a.underlying()), RawData(1));
    } else {
        memory.write(Address(a.underlying()), RawData(0));
    }
}

void print_eq(std::ostream& ostream, RawData a, RawData b, RawData c)
{
    ostream << "EQ ";

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
