#include "crosscutting/typedefs.h"
#include "instruction_util.h"
#include <iostream>

namespace emu::memory {
    class EmulatorMemory;
}

namespace emu::z80 {

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
     * @param memory is the memory, which will be mutated
     * @param iff1 is the first interrupt flag
     * @param iff2 is the second interrupt flag
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void retn(u16 &pc, u16 &sp, const EmulatorMemory &memory, bool &iff1, bool iff2, cyc &cycles) {
        iff1 = iff2;
        execute_return(pc, sp, memory);

        cycles = 14;
    }

    void print_retn(std::ostream &ostream) {
        ostream << "RETN";
    }
}
