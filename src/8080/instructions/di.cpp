#include <iostream>

namespace emu::cpu8080 {
    /**
     * Disable interrupts
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
    void di(bool &inte, unsigned long &cycles) {
        inte = false;

        cycles = 4;
    }

    void print_di(std::ostream& ostream) {
        ostream << "DI";
    }
}
