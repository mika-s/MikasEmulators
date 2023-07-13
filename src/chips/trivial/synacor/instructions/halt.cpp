#include <iostream>

namespace emu::synacor {

/**
 * Stop execution and terminate the program
 * <ul>
 *   <li>Size: 1</li>
 * </ul>
 *
 * @param is_halted is the halted status, which will be mutated
 */
void halt(bool& is_halted)
{
    is_halted = true;
}

void print_halt(std::ostream& ostream)
{
    ostream << "HALT";
}
}
