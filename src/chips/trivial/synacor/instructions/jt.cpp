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
 * @param pc is the program counter, which will be modified
 * @param memory is the memory
 * @param a is the value to check for nonzero
 * @param b is the address to jump to
 */
void jt(Address& pc, EmulatorMemory<Address, RawData> const& memory, RawData a, RawData b)
{
    if (a != RawData(0)) {
        if (b >= RawData(32768)) {
            pc = Address(memory.read(Address(b.underlying())));
        } else {
            pc = Address(b.underlying());
        }
    }
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
