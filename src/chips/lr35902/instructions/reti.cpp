#include "crosscutting/typedefs.h"
#include "instruction_util.h"
#include <iostream>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lr35902 {

/**
 * Return from interrupt
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4/li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory
 * @param cycles is the number of cycles variable, which will be mutated
 */
void reti(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory, cyc& cycles)
{
    execute_return(pc, sp, memory);

    cycles = 16;
}

void print_reti(std::ostream& ostream)
{
    ostream << "RETI";
}
}
