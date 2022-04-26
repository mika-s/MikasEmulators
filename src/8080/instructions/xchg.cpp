#include <cstdint>
#include <iostream>
#include "doctest.h"

namespace emu::cpu8080 {
    /**
     * Exchange H and L with D and E
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param d_reg is the D register, which will be mutated
     * @param e_reg is the E register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xchg(std::uint8_t &h_reg, std::uint8_t &l_reg, std::uint8_t &d_reg, std::uint8_t &e_reg, unsigned long &cycles) {
        const std::uint8_t h = h_reg;
        const std::uint8_t l = l_reg;
        h_reg = d_reg;
        l_reg = e_reg;
        d_reg = h;
        e_reg = l;

        cycles = 4;
    }

    void print_xchg(std::ostream &ostream) {
        ostream << "XCHG";
    }

    TEST_CASE("8080: XCHG") {
        unsigned long cycles = 0;

        SUBCASE("should exchange HL with DE") {
            std::uint8_t h_reg = 0x11;
            std::uint8_t l_reg = 0x22;
            std::uint8_t d_reg = 0x33;
            std::uint8_t e_reg = 0x44;

            xchg(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(0x33, h_reg);
            CHECK_EQ(0x44, l_reg);
            CHECK_EQ(0x11, d_reg);
            CHECK_EQ(0x22, e_reg);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            std::uint8_t h_reg = 0x11;
            std::uint8_t l_reg = 0x22;
            std::uint8_t d_reg = 0;
            std::uint8_t e_reg = 0;

            xchg(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
