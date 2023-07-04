#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Read a character from the terminal and write its ascii code to <a>
 * <br>
 * It can be assumed that once input starts, it will continue until a
 * newline is encountered; this means that you can safely read whole
 * lines from the keyboard instead of having to figure out how to read
 * individual characters.
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param a is address to write input to
 * @param memory is the memory, which will be mutated
 */
void in(Address a, EmulatorMemory<Address, Data>& memory)
{
    memory.write(a, Data(1)); // TODO
}

void print_in(std::ostream& ostream, Address a)
{
    ostream << "IN "
            << hexify(static_cast<u16>(a.underlying()));
}
}
