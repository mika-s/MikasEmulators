#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Subtract
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        sub(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Subtract
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        sub_from_register(acc_reg, value, false, flag_reg);

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_sub(const std::string &reg) {
        std::cout << "SUB " << reg;
    }
}
