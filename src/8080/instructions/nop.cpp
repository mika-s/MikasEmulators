#include <iostream>

namespace emu::cpu8080 {
    /**
     * No operation
     * <ul>
     *   <li>Size: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void nop(unsigned long &cycles) {
        cycles = 4;
    }

    void print_nop() {
        std::cout << "NOP";
    }
}
