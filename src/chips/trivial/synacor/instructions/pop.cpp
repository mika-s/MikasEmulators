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
 * Remove the top element from the stack and write it into <a>; empty stack = error
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param stack is the stack, which will be mutated
 * @param a is the place to write the top element to, which will be mutated
 */
void pop(std::stack<Address>& stack, EmulatorMemory<Address, RawData>& memory, RawData a)
{
    if (stack.empty()) {
        throw std::runtime_error("The stack is empty");
    }

    if (a >= RawData(32768)) {
        auto const stack_value = RawData(stack.top().underlying());
        stack.pop();
        memory.write(Address(a.underlying()), stack_value);
    } else {
        throw std::runtime_error("Cannot POP to a non-register");
    }
}

void print_pop(std::ostream& ostream, RawData a)
{
    ostream << "POP ";

    if (a >= RawData(32768)) {
        ostream << "r" << static_cast<u16>(Data(a.underlying()).underlying());
    } else {
        ostream << hexify(static_cast<u16>(a.underlying()));
    }
}
}
