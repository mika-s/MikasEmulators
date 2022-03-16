#include <cstdint>
#include <gtest/gtest.h>
#include "flags.h"
#include "instructions.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(InrTest, ShouldIncreaseRegisterOrMemory) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        Flags flag_reg;

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(1, reg);

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(2, reg);

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(3, reg);

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(4, reg);

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(5, reg);
    }

    TEST(InrTest, ShouldNotAffectCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 255;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_carry_flag_set());

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(0, reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(InrTest, ShouldSetCorrectAuxCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 15;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(16, reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(InrTest, ShouldSetCorrectParityInParityFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        Flags flag_reg;

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());

        inr(reg, flag_reg, cycles);
        inr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(InrTest, ShouldSetCorrectValueInZeroFlag) {
        unsigned long cycles = 0;

        std::uint8_t reg = 254;
        Flags flag_reg;

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_zero_flag_set());
    }

    TEST(InrTest, ShouldSetSignFlagWhenGoingAbove127) {
        unsigned long cycles = 0;

        std::uint8_t reg = 127;
        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_sign_flag_set());

        inr(reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_sign_flag_set());
    }

    TEST(InrTest, ShouldUse5CyclesWhenMemoryIsNotInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t reg = 0xE;
        inr(reg, flag_reg, cycles);
        EXPECT_EQ(5, cycles);
    }

    TEST(InrTest, ShouldUse10CyclesWhenMemoryIsInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t reg = 0xE;
        inr(reg, flag_reg, cycles, true);
        EXPECT_EQ(10, cycles);
    }
}
