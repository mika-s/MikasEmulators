#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "flags.h"
#include <iostream>

namespace emu::lmc {

using emu::memory::EmulatorMemory;

/**
 * Subtract value in memory from the accumulator
 * <ul>
 *   <li>Size: 2</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param address is the address to the value in memory
 * @param memory is the memory
 * @param flag_reg is the flag register, which will be mutated
 */
void sub(Data& acc_reg, Address address, EmulatorMemory<Address, Data> const& memory, Flags& flag_reg)
{
    flag_reg.handle_negative_flag(acc_reg, memory.read(address));

    acc_reg -= memory.read(address);
}

void print_sub(std::ostream& ostream, Address address)
{
    ostream << "SUB "
            << address;
}
}
