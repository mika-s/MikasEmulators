#include <cstdint>
#include <iostream>

namespace emu::cpu8080 {
    /**
     * Move H&L to program counter
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param address is the address in the HL register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pchl(std::uint16_t &pc, std::uint16_t address, unsigned long &cycles) {
        pc = address;

        cycles = 5;
    }

    void print_pchl() {
        std::cout << "PCHL";
    }
}
