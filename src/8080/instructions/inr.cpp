#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080 {
    /**
     * Increment
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to increment, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inr(u8 &reg, Flags &flag_reg, unsigned long &cycles) {
        inr(reg, flag_reg, cycles, false);
    }

    /**
     * Increment
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to increment, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inr(u8 &reg, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        const u8 previous = reg;
        reg++;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_parity_flag(reg);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_aux_carry_flag(previous, 1, false);

        cycles = 5;

        if (is_memory_involved) {
            cycles += 5;
        }
    }

    void print_inr(std::ostream &ostream, const std::string &reg) {
        ostream << "INR "
                << reg;
    }

    TEST_CASE("8080: INR") {
        unsigned long cycles = 0;

        SUBCASE("should increase register or memory") {
            u8 reg = 0;
            Flags flag_reg;

            inr(reg, flag_reg, cycles);
            CHECK_EQ(1, reg);

            inr(reg, flag_reg, cycles);
            CHECK_EQ(2, reg);

            inr(reg, flag_reg, cycles);
            CHECK_EQ(3, reg);

            inr(reg, flag_reg, cycles);
            CHECK_EQ(4, reg);

            inr(reg, flag_reg, cycles);
            CHECK_EQ(5, reg);
        }

        SUBCASE("should not affect the carry flag") {
            u8 reg = 255;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            inr(reg, flag_reg, cycles);

            CHECK_EQ(0, reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set correct aux carry flag") {
            u8 reg = 15;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());

            inr(reg, flag_reg, cycles);

            CHECK_EQ(16, reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should set correct parity in the parity flag") {
            u8 reg = 0;
            Flags flag_reg;

            inr(reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_parity_flag_set());

            inr(reg, flag_reg, cycles);
            inr(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set correct value in the zero flag") {
            u8 reg = 254;
            Flags flag_reg;

            inr(reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            inr(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when going above 127") {
            u8 reg = 127;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_sign_flag_set());

            inr(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_sign_flag_set());
        }

        SUBCASE("should use 5 cycles when memory is not involved") {
            cycles = 0;
            Flags flag_reg;

            u8 reg = 0xE;
            inr(reg, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 10 cycles when memory is involved") {
            cycles = 0;
            Flags flag_reg;

            u8 reg = 0xE;
            inr(reg, flag_reg, cycles, true);

            CHECK_EQ(10, cycles);
        }
    }
}
