#include <cstdint>
#include <iostream>
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Return from subroutine
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret(std::uint16_t &pc, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
        execute_return(pc, sp, memory);

        cycles = 10;
    }

    void print_ret(std::ostream& ostream) {
        ostream << "RET";
    }
}
