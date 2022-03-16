#include <iostream>

namespace emu::cpu8080 {
    /**
     * Halt
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param stopped is the stopped status variable
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void hlt(bool &stopped, unsigned long &cycles) {
        stopped = true;

        cycles = 7;
    }

    void print_hlt() {
        std::cout << "HLT";
    }
}
