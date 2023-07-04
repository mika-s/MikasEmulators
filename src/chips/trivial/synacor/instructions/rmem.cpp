#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include <iostream>

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::util::string::hexify;

/**
 * Read memory at address <b> and write it to <a>
 * <ul>
 *   <li>Size: 3</li>
 * </ul>
 *
 * @param a is the address to write to
 * @param b is the address to read from
 * @param memory is the memory
 */
void rmem(Address a, Address b, EmulatorMemory<Address, Data>& memory)
{
    memory.write(a, memory.read(b));
}

void print_rmem(std::ostream& ostream, Address a, Address b)
{
    ostream << "RMEM "
            << hexify(static_cast<u16>(a.underlying()))
            << " "
            << hexify(static_cast<u16>(b.underlying()));
}
}
