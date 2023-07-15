#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Jump to <a>
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param pc is the program counter, which will be modified
 * @param memory is the memory
 * @param a is the address to jump to
 */
void jmp(Address& pc, EmulatorMemory<Address, RawData> const& memory, RawData a)
{
    const RawData real_a = a >= RawData(32768) ? memory.read(Address(a)) : a;

    pc = Address(real_a.underlying());
}

void print_jmp(std::ostream& ostream, RawData a)
{
    ostream << "JMP ";

    if (a >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(a.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(a.underlying()));
    }
}
}
