#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * If <a> is nonzero, jump to <b>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param a is the accumulator register, which will be mutated
 * @param b is the address to the value in memory
 */
void jt()
{
}

void print_jt(std::ostream& ostream, RawData a, RawData b)
{
    ostream << "JT ";

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
