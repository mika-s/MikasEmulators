#include <cstdint>
#include <iostream>

namespace emu::cpu8080 {
    /**
     * Move H&L to SP
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param address_in_hl is the address stored in the HL registers
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sphl(std::uint16_t &sp, std::uint16_t address_in_hl, unsigned long &cycles) {
        sp = address_in_hl;

        cycles = 5;
    }

    void print_sphl() {
        std::cout << "SPHL";
    }
}
