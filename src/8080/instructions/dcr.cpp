#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Decrement
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dcr(std::uint8_t &reg, Flags &flag_reg, unsigned long &cycles) {
        dcr(reg, flag_reg, cycles, false);
    }

    /**
     * Decrement
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void dcr(std::uint8_t &reg, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        const std::uint8_t previous = reg;
        reg--;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_parity_flag(reg);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_aux_borrow_flag(previous, 1, false);

        cycles = 5;

        if (is_memory_involved) {
            cycles += 5;
        }
    }

    void print_dcr(const std::string &reg) {
        std::cout << "DCR " << reg;
    }
}
