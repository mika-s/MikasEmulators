#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Read memory at address <b> and write it to <a>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param a is the address to write to
 * @param b is the address to read from
 * @param memory is the memory
 */
void rmem(Address a, Address b, EmulatorMemory<Address, RawData>& memory)
{
    memory.write(a, memory.read(b));
}

void print_rmem(std::ostream& ostream, RawData a, RawData b)
{
    ostream << "RMEM ";

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
