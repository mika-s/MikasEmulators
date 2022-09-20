#include <iostream>
#include "doctest.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"

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
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void reti(u16 &pc, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles) {
        execute_return(pc, sp, memory);
        // TODO: Signal an I/O device that the interrupt routine is completed.

        cycles = 14;
    }

    void print_reti(std::ostream &ostream) {
        ostream << "RETI";
    }
}
