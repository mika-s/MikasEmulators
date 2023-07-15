#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Set register <a> to the value of <b>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param a is the <a> register
 * @param b is the value to place into <a>
 */
void set(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b)
{
    memory.write(Address(a.underlying()), b);
}

void print_set(std::ostream& ostream, RawData a, RawData b)
{
    ostream << "SET ";

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
