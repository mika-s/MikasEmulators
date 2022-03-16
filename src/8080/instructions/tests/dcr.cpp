#include <cstdint>
#include <gtest/gtest.h>
#include "flags.h"
#include "instructions.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(DcrTest, ShouldDecreaseRegisterOrMemory) {
        unsigned long cycles = 0;

        std::uint8_t reg = 10;
        Flags flag_reg;

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(9, reg);

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(8, reg);

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(7, reg);

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(6, reg);

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(5, reg);
    }

    TEST(DcrTest, ShouldNotAffectCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_carry_flag_set());

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(255, reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(DcrTest, ShouldSetCorrectAuxCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 15;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(14, reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(DcrTest, ShouldSetCorrectParityInParityFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 5;
        Flags flag_reg;

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(DcrTest, ShouldSetCorrectValueInZeroFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 2;
        Flags flag_reg;

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_zero_flag_set());
    }

    TEST(DcrTest, ShouldSetSignFlagWhenGoingAbove127) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_sign_flag_set());

        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_sign_flag_set());
    }

    TEST(DcrTest, ShouldUse5CyclesWhenMemoryIsNotInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t reg = 0xE;
        dcr(reg, flag_reg, cycles);
        EXPECT_EQ(5, cycles);
    }

    TEST(DcrTest, ShouldUse10CyclesWhenMemoryIsInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t reg = 0xE;
        dcr(reg, flag_reg, cycles, true);
        EXPECT_EQ(10, cycles);
    }
}
