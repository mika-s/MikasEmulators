#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    /**
     * Exchange registers
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex(u8 &acc_reg, Flags &flag_reg, u8 &acc_p_reg, Flags &flag_p_reg, unsigned long &cycles) {
        const u8 acc_temp = acc_reg;
        acc_reg = acc_p_reg;
        acc_p_reg = acc_temp;

        Flags flags_temp;
        flags_temp = flag_reg;
        flag_reg = flag_p_reg;
        flag_p_reg = flags_temp;

        cycles = 4;
    }

    /**
     * Exchange H&L with top of stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 19</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param sp0 is the top of the stack, which will be mutated
     * @param sp1 is the second from the top of the stack, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex_sp_hl(u8 &h_reg, u8 &l_reg, u8 &sp0, u8 &sp1, unsigned long &cycles) {
        const u8 h = h_reg;
        const u8 l = l_reg;
        l_reg = sp0;
        sp0 = l;
        h_reg = sp1;
        sp1 = h;

        cycles = 19;
    }

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
    void ex_de_hl(u8 &h_reg, u8 &l_reg, u8 &d_reg, u8 &e_reg, unsigned long &cycles) {
        const u8 h = h_reg;
        const u8 l = l_reg;
        h_reg = d_reg;
        l_reg = e_reg;
        d_reg = h;
        e_reg = l;

        cycles = 4;
    }

    void print_ex(std::ostream &ostream, const std::string &reg, const std::string &comp_reg) {
        ostream << "EX "
                << reg
                << ","
                << comp_reg;
    }

    TEST_CASE("Z80: EX") {
        unsigned long cycles = 0;

        SUBCASE("should exchange AF with AF'") {
            cycles = 0;

            u8 acc_reg = 10;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            u8 acc_p_reg = 4;
            Flags flag_p_reg;
            flag_p_reg.set_half_carry_flag();

            ex(acc_reg, flag_reg, acc_p_reg, flag_p_reg, cycles);

            CHECK_EQ(4, acc_reg);
            CHECK_EQ(10, acc_p_reg);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());

            CHECK_EQ(true, flag_p_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_p_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_p_reg.is_add_subtract_flag_set());
            CHECK_EQ(false, flag_p_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_p_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_p_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            u8 acc_reg = 10;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            u8 acc_p_reg = 4;
            Flags flag_p_reg;
            flag_reg.set_half_carry_flag();

            ex(acc_reg, flag_reg, acc_p_reg, flag_p_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: EX (SP), HL") {
        unsigned long cycles = 0;

        SUBCASE("should exchange HL with top of the stack") {
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 sp0 = 0x33;
            u8 sp1 = 0x44;

            ex_sp_hl(h_reg, l_reg, sp0, sp1, cycles);

            CHECK_EQ(0x44, h_reg);
            CHECK_EQ(0x33, l_reg);
            CHECK_EQ(0x22, sp0);
            CHECK_EQ(0x11, sp1);
        }

        SUBCASE("should use 18 cycles") {
            cycles = 0;
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 sp0 = 0;
            u8 sp1 = 0;

            ex_sp_hl(h_reg, l_reg, sp0, sp1, cycles);

            CHECK_EQ(19, cycles);
        }
    }

    TEST_CASE("Z80: EX DE, HL") {
        unsigned long cycles = 0;

        SUBCASE("should exchange HL with DE") {
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 d_reg = 0x33;
            u8 e_reg = 0x44;

            ex_de_hl(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(0x33, h_reg);
            CHECK_EQ(0x44, l_reg);
            CHECK_EQ(0x11, d_reg);
            CHECK_EQ(0x22, e_reg);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 d_reg = 0;
            u8 e_reg = 0;

            ex_de_hl(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
