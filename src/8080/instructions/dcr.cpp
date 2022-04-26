#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Decrement
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dcr(std::uint8_t &reg, Flags &flag_reg, unsigned long &cycles) {
        dcr(reg, flag_reg, cycles, false);
    }

    /**
     * Decrement
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 10</li>
     *   <li>Condition bits affected: auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void dcr(std::uint8_t &reg, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        const std::uint8_t previous = reg;
        reg--;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_parity_flag(reg);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_aux_borrow_flag(previous, 1, false);

        cycles = 5;

        if (is_memory_involved) {
            cycles += 5;
        }
    }

    void print_dcr(std::ostream &ostream, const std::string &reg) {
        ostream << "DCR "
                << reg;
    }

    TEST_CASE("8080: DCR") {
        unsigned long cycles = 0;

        SUBCASE("should decrease register or memory") {
            std::uint8_t reg = 10;
            Flags flag_reg;

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(9, reg);

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(8, reg);

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(7, reg);

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(6, reg);

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(5, reg);
        }

        SUBCASE("should not affect the carry flag") {
            std::uint8_t reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            dcr(reg, flag_reg, cycles);

            CHECK_EQ(255, reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set correct aux carry flag") {
            std::uint8_t reg = 15;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());

            dcr(reg, flag_reg, cycles);

            CHECK_EQ(14, reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should set correct parity in the parity flag") {
            std::uint8_t reg = 5;
            Flags flag_reg;

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set correct value in the zero flag") {
            std::uint8_t reg = 2;
            Flags flag_reg;

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            dcr(reg, flag_reg, cycles);
            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when going above 127") {
            std::uint8_t reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_sign_flag_set());

            dcr(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_sign_flag_set());
        }

        SUBCASE("should use 5 cycles when memory is not involved") {
            cycles = 0;
            Flags flag_reg;
            std::uint8_t reg = 0xe;

            dcr(reg, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 10 cycles when memory is involved") {
            cycles = 0;
            Flags flag_reg;
            std::uint8_t reg = 0xe;

            dcr(reg, flag_reg, cycles, true);

            CHECK_EQ(10, cycles);
        }
    }
}
