#include "chips/trivial/synacor/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::util::string::hexify;

/**
 * Write the character represented by ascii code <a> to the terminal
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param character is the character to print to the terminal
 */
void out(Data character)
{
    std::cout << static_cast<char>(character.underlying())
              << std::flush;
}

std::string as_str(Data character)
{
    if (character.underlying() == 0x000a) {
        return "\\n";
    } else {
        return { 1, static_cast<char>(character.underlying()) };
    }
}

void print_out(std::ostream& ostream, Data character)
{
    ostream << "OUT "
            << hexify(static_cast<u16>(character.underlying()))
            << " ("
            << as_str(character)
            << ")";
}
}
