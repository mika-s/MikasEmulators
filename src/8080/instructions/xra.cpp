#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Exclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be exclusive ored with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xra(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        xra(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Exclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be exclusive ored with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void xra(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        acc_reg ^= value;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by XRA, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but should be reset according to the 8080/8085 Assembly Language Programming
        // Manual. It is reset in this emulator.

        flag_reg.clear_aux_carry_flag();

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_xra(const std::string &reg) {
        std::cout << "XRA " << reg;
    }
}
