#include "crosscutting/typedefs.h"
#include "instruction_util.h"
#include <iostream>

namespace emu::memory {
    class EmulatorMemory;
}

namespace emu::z80 {

    /**
     * Return from interrupt
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
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void reti(u16 &pc, u16 &sp, const EmulatorMemory &memory, cyc &cycles) {
        execute_return(pc, sp, memory);
        // TODO: Signal an I/O device that the interrupt routine is completed.

        cycles = 14;
    }

    void print_reti(std::ostream &ostream) {
        ostream << "RETI";
    }
}
