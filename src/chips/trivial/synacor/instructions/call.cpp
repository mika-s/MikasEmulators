#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Write the address of the next instruction to the stack and jump to <a>
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param address is the address to the value in memory
 * @param memory is the memory
 */
void call()
{
}

void print_call(std::ostream& ostream, RawData a)
{
    ostream << "CALL ";

    if (a >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(a.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(a.underlying()));
    }
}
}
