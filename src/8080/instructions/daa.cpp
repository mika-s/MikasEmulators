#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Decimal adjust accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void daa(std::uint8_t &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        std::uint8_t lower_nibble = acc_reg & 0x0F;
        std::uint8_t upper_nibble = acc_reg >> 4;

        bool carry = flag_reg.is_carry_flag_set();
        std::uint8_t value_to_add = 0;

        if (lower_nibble > 9 || flag_reg.is_aux_carry_flag_set()) {
            value_to_add = 6;
        }

        if (upper_nibble > 9 || carry || (lower_nibble > 9 && upper_nibble > 8)) {
            value_to_add += 6 << 4;
            carry = true;
        }

        add_to_register(acc_reg, value_to_add, false, flag_reg);

        if (carry) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        cycles = 4;
    }

    void print_daa() {
        std::cout << "DAA";
    }
}
