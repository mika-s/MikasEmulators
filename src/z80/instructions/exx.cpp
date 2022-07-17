#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    /**
     * Exchange registers BC <--> BC', DE <--> DE', HL <--> HL'
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param b_reg is the b register, which will be mutated
     * @param c_reg is the c register, which will be mutated
     * @param b_p_reg is the b' register, which will be mutated
     * @param c_p_reg is the c' register, which will be mutated
     * @param d_reg is the d register, which will be mutated
     * @param e_reg is the e register, which will be mutated
     * @param d_p_reg is the d' register, which will be mutated
     * @param e_p_reg is the e' register, which will be mutated
     * @param h_reg is the h register, which will be mutated
     * @param l_reg is the l register, which will be mutated
     * @param h_p_reg is the h' register, which will be mutated
     * @param l_p_reg is the l' register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void exx(
            u8 &b_reg, u8 &c_reg, u8 &b_p_reg, u8 &c_p_reg,
            u8 &d_reg, u8 &e_reg, u8 &d_p_reg, u8 &e_p_reg,
            u8 &h_reg, u8 &l_reg, u8 &h_p_reg, u8 &l_p_reg,
            unsigned long &cycles
    ) {
        u8 temp = b_reg;
        b_reg = b_p_reg;
        b_p_reg = temp;

        temp = c_reg;
        c_reg = c_p_reg;
        c_p_reg = temp;

        temp = d_reg;
        d_reg = d_p_reg;
        d_p_reg = temp;

        temp = e_reg;
        e_reg = e_p_reg;
        e_p_reg = temp;

        temp = h_reg;
        h_reg = h_p_reg;
        h_p_reg = temp;

        temp = l_reg;
        l_reg = l_p_reg;
        l_p_reg = temp;

        cycles = 4;
    }

    void print_exx(std::ostream &ostream) {
        ostream << "EXX";
    }

    TEST_CASE("Z80: EXX") {
        unsigned long cycles = 0;

        SUBCASE("should exchange AF with AF'") {
            cycles = 0;

            u8 b_reg = 10;
            u8 c_reg = 5;
            u8 b_p_reg = 56;
            u8 c_p_reg = 72;

            u8 d_reg = 88;
            u8 e_reg = 22;
            u8 d_p_reg = 123;
            u8 e_p_reg = 54;

            u8 h_reg = 0;
            u8 l_reg = 0;
            u8 h_p_reg = 90;
            u8 l_p_reg = 234;

            exx(
                    b_reg, c_reg, b_p_reg, c_p_reg,
                    d_reg, e_reg, d_p_reg, e_p_reg,
                    h_reg, l_reg, h_p_reg, l_p_reg,
                    cycles
            );

            CHECK_EQ(56, b_reg);
            CHECK_EQ(72, c_reg);
            CHECK_EQ(10, b_p_reg);
            CHECK_EQ(5, c_p_reg);

            CHECK_EQ(123, d_reg);
            CHECK_EQ(54, e_reg);
            CHECK_EQ(88, d_p_reg);
            CHECK_EQ(22, e_p_reg);

            CHECK_EQ(90, h_reg);
            CHECK_EQ(234, l_reg);
            CHECK_EQ(0, h_p_reg);
            CHECK_EQ(0, l_p_reg);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            u8 b_reg = 10;
            u8 c_reg = 5;
            u8 b_p_reg = 56;
            u8 c_p_reg = 72;

            u8 d_reg = 88;
            u8 e_reg = 22;
            u8 d_p_reg = 123;
            u8 e_p_reg = 54;

            u8 h_reg = 0;
            u8 l_reg = 0;
            u8 h_p_reg = 90;
            u8 l_p_reg = 234;

            exx(
                    b_reg, c_reg, b_p_reg, c_p_reg,
                    d_reg, e_reg, d_p_reg, e_p_reg,
                    h_reg, l_reg, h_p_reg, l_p_reg,
                    cycles
            );

            CHECK_EQ(4, cycles);
        }
    }
}
