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
 * Push <a> onto the stack
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param stack is the stack, which will be mutated
 * @param a is the value to push onto the stack
 */
void push(std::stack<Data>& stack, RawData a)
{
    stack.emplace(a.underlying());
}

void print_push(std::ostream& ostream, RawData a)
{
    ostream << "PUSH ";

    if (a >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(a.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(a.underlying()));
    }
}
}
