#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::synacor {

/**
 * No operation
 * <ul>
 *   <li>Size: 1</li>
 * </ul>
 */
void noop()
{

}

void print_noop(std::ostream& ostream)
{
    ostream << "NOOP";
}
}
