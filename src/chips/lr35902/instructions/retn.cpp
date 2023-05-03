#include "crosscutting/typedefs.h"
#include "instruction_util.h"
#include <iostream>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lr35902 {

/**
 * Return from nonmaskable interrupt
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4/li>
 *   <li>States: 14</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory
 * @param iff1 is the first interrupt flag, which will be mutated
 * @param iff2 is the second interrupt flag
 * @param cycles is the number of cycles variable, which will be mutated
 */
void retn(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory, bool& iff1, bool iff2, cyc& cycles)
{
    iff1 = iff2;
    execute_return(pc, sp, memory);

    cycles = 14;
}

void print_retn(std::ostream& ostream)
{
    ostream << "RETN";
}
}
