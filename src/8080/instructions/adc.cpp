#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080 {
    /**
     * Add with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        adc(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Add with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void adc(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        add_to_register(acc_reg, value, flag_reg.is_carry_flag_set(), flag_reg);

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_adc(const std::string &reg) {
        std::cout << "ADC " << reg;
    }
}
