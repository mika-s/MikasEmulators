#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>
#include <stack>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Write the address of the next instruction to the stack and jump to <a>
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param stack is the stack, which will be mutated
 * @param a is the address to jump to
 */
void call(Address& pc, std::stack<Address>& stack, RawData a)
{
    stack.emplace(pc.underlying() + 1);
    pc = Address(a.underlying());
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
