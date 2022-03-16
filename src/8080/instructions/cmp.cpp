#include <cstdint>
#include <iostream>
#include "flags.h"
#include "instructions.h"

namespace emu::cpu8080 {
    /**
     * Compare with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cmp(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        cmp(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Compare with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void cmp(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        const std::uint8_t previous = acc_reg;
        const std::uint8_t new_acc_reg = previous - value;

        flag_reg.handle_borrow_flag(previous, value);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_aux_borrow_flag(previous, value, false);

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_cmp(const std::string &reg) {
        std::cout << "CMP " << reg;
    }
}
