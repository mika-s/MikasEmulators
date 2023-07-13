#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::util::string::hexify;

/**
 * Store into <a> the remainder of <b> divided by <c>
 * <ul>
 *   <li>Size: 4</li>
 * </ul>
 *
 * @param a is the program counter, which will be mutated
 * @param b is the address to the value in memory
 * @param c is the flag register
 */
void mod()
{
}

void print_mod(std::ostream& ostream, RawData a, RawData b, RawData c)
{
    ostream << "MOD ";

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
