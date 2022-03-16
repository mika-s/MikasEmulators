#include <cstdint>
#include <iostream>
#include "byte_util.h"
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Rotate accumulator left
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
    void rlc(std::uint8_t &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        const bool should_set_carry_flag = emu::util::byte::is_bit_set(acc_reg, HIGH_BIT);
        acc_reg = acc_reg << 1;
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
            emu::util::byte::set_bit(acc_reg, LOW_BIT);
        } else {
            flag_reg.clear_carry_flag();
        }

        cycles = 4;
    }

    void print_rlc(const std::string &reg) {
        std::cout << "RLC " << reg;
    }
}
