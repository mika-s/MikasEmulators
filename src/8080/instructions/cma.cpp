#include <cstdint>
#include <iostream>

namespace emu::cpu8080 {
    /**
     * Complement accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cma(std::uint8_t &acc_reg, unsigned long &cycles) {
        acc_reg = ~acc_reg;

        cycles = 4;
    }

    void print_cma(std::ostream& ostream) {
        ostream << "CMA";
    }
}
