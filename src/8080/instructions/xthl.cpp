#include <cstdint>
#include <iostream>
#include "doctest.h"

namespace emu::cpu8080 {
    /**
     * Exchange H&L with top of stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 18</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param sp0 is the top of the stack, which will be mutated
     * @param sp1 is the second from the top of the stack, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xthl(std::uint8_t &h_reg, std::uint8_t &l_reg, std::uint8_t &sp0, std::uint8_t &sp1, unsigned long &cycles) {
        const std::uint8_t h = h_reg;
        const std::uint8_t l = l_reg;
        l_reg = sp0;
        sp0 = l;
        h_reg = sp1;
        sp1 = h;

        cycles = 18;
    }

    void print_xthl(std::ostream &ostream) {
        ostream << "XTHL";
    }

    TEST_CASE("8080: XTHL") {
        unsigned long cycles = 0;

        SUBCASE("should exchange HL with top of the stack") {
            std::uint8_t h_reg = 0x11;
            std::uint8_t l_reg = 0x22;
            std::uint8_t sp0 = 0x33;
            std::uint8_t sp1 = 0x44;

            xthl(h_reg, l_reg, sp0, sp1, cycles);

            CHECK_EQ(0x44, h_reg);
            CHECK_EQ(0x33, l_reg);
            CHECK_EQ(0x22, sp0);
            CHECK_EQ(0x11, sp1);
        }

        SUBCASE("should use 18 cycles") {
            cycles = 0;
            std::uint8_t h_reg = 0x11;
            std::uint8_t l_reg = 0x22;
            std::uint8_t sp0 = 0;
            std::uint8_t sp1 = 0;

            xthl(h_reg, l_reg, sp0, sp1, cycles);

            CHECK_EQ(18, cycles);
        }
    }
}
