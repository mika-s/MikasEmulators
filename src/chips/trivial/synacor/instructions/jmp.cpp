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
 * @param a is the accumulator register, which will be mutated
 */
void jmp()
{
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
