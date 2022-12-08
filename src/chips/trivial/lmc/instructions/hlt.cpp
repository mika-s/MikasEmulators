#include <iostream>

namespace emu::lmc {

/**
 * Halt the program
 * <ul>
 *   <li>Size: 1</li>
 * </ul>
 *
 * @param is_halted is the halted status, which will be mutated
 */
void hlt(bool& is_halted)
{
    is_halted = true;
}

void print_hlt(std::ostream& ostream)
{
    ostream << "HLT";
}
}
