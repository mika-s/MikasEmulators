#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    /**
     * Rotate right through carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rar(std::uint8_t &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        const bool should_set_carry_flag = emu::util::byte::is_bit_set(acc_reg, LOW_BIT);
        acc_reg = acc_reg >> 1;
        if (flag_reg.is_carry_flag_set()) {
            emu::util::byte::set_bit(acc_reg, HIGH_BIT);
        }
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        cycles = 4;
    }

    void print_rar() {
        std::cout << "RAR";
    }
}
