#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Inclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ora(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        ora(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Inclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ora(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        acc_reg |= value;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by ORA, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but should be reset according to the 8080/8085 Assembly Language Programming
        // Manual. It is reset in this emulator.

        flag_reg.clear_aux_carry_flag();

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_ora(std::ostream& ostream, const std::string &reg) {
        ostream << "ORA "
                << reg;
    }
}
