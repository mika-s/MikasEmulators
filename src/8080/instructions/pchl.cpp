#include <cstdint>
#include <iostream>
#include "doctest.h"

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

    void print_pchl(std::ostream &ostream) {
        ostream << "PCHL";
    }

    TEST_CASE("8080: PCHL") {
        unsigned long cycles = 0;
        std::uint16_t pc = 0x1111;
        std::uint16_t address = 0x432a;

        SUBCASE("should load address into PC") {
            pchl(pc, address, cycles);

            CHECK_EQ(address, pc);
        }

        SUBCASE("should use 5 cycles") {
            cycles = 0;

            pchl(pc, address, cycles);

            CHECK_EQ(5, cycles);
        }
    }
}
