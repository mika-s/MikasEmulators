#include <cstdint>
#include <gtest/gtest.h>
#include "byte_util.h"
#include "flags.h"
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(DaaTest, ShouldDecimalAdjustTheAccumulator) {
        // Example from the 8080 Programmers Manual.
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x9b;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(0x01, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(DaaTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x9a;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_zero_flag_set());

        acc_reg = 0x9b;
        flag_reg.reset();

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());
    }

    TEST(DaaTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x9a;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
    }

    TEST(DaaTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x9a;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(DaaTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x9b;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(DaaTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xFF;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(DaaTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        Flags flag_reg;

        daa(acc_reg, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
