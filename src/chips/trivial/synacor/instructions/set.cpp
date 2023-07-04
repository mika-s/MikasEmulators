#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::util::string::hexify;

/**
 * set register <a> to the value of <b>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param a is the <a> register, which will be mutated
 * @param b is the value to place into <a>
 */
void set([[maybe_unused]] Data& a, [[maybe_unused]] Data b)
{
}

void print_set(std::ostream& ostream, Data a, Data b)
{
    ostream << "SET "
            << hexify(static_cast<u16>(a.underlying()))
            << " "
            << hexify(static_cast<u16>(b.underlying()));
}
}
