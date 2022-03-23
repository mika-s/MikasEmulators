#include <iostream>
#include "8080/flags.h"

namespace emu::cpu8080 {
    /**
     * Complement carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry</li>
     * </ul>
     *
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cmc(Flags &flag_reg, unsigned long &cycles) {
        flag_reg.toggle_carry_flag();

        cycles = 4;
    }

    void print_cmc() {
        std::cout << "CMC";
    }
}
