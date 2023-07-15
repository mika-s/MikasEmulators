#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>
#include <stack>

namespace emu::synacor {

/**
 * Remove the top element from the stack and jump to it; empty stack = halt
 * <ul>
 *   <li>Size: 1</li>
 * </ul>
 *
 * @param stack is the stack, which will be mutated
 * @param pc is the program counter, which will be mutated
 * @param is_halted is the halted status, which will be mutated
 */
void ret(std::stack<Address>& stack, Address& pc, bool& is_halted)
{
    if (stack.empty()) {
        is_halted = true;
        return;
    }

    pc = stack.top();
    stack.pop();
}

void print_ret(std::ostream& ostream)
{
    ostream << "RET";
}
}
