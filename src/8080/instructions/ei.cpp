#include <iostream>

namespace emu::cpu8080 {
    /**
     * Enable interrupts
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param inte is the interrupt bit, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ei(bool &inte, unsigned long &cycles) {
        inte = true;

        cycles = 4;
    }

    void print_ei(std::ostream& ostream) {
        ostream << "EI";
    }
}
