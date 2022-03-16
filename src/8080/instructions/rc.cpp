#include <cstdint>
#include <iostream>
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Return if carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rc(std::uint16_t &pc, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory, const Flags &flag_reg,
            unsigned long &cycles) {
        cycles = 0;

        if (flag_reg.is_carry_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    void print_rc() {
        std::cout << "RC";
    }
}
