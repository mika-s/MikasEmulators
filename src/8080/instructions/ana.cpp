#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Logical and with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be anded with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ana(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        ana(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Logical and with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be anded with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void ana(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        const std::uint8_t previous = acc_reg;
        acc_reg &= value;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by ANA, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but apparently has to be modified anyway. This is explained here:
        // https://demin.ws/blog/english/2012/12/24/my-i8080-collection/
        // The 8080/8085 Assembly Language Programming Manual also mentions auxiliary carry being modified by ANI.

        if (((previous | value) & 0x08) != 0) {
            flag_reg.set_aux_carry_flag();
        } else {
            flag_reg.clear_aux_carry_flag();
        }

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_ana(std::ostream& ostream, const std::string &reg) {
        ostream << "ANA "
                << reg;
    }
}
