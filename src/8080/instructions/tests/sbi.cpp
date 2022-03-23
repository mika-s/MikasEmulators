#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"
#include "8080/flags.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(SbiTest, ShouldSubtractGivenValueFromAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                NextByte args = { value };
                sbi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter - value), acc_reg);
            }
        }
    }

    TEST(SbiTest, ShouldSubtractGivenValueToAccumulatorWithTakingCarryIntoAccount) {
        unsigned long cycles = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        std::uint8_t acc_reg = 0xac;
        NextByte args = { 0xab };
        sbi(acc_reg, args , flag_reg, cycles);
        EXPECT_EQ(0, acc_reg);
    }

    TEST(SbiTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                NextByte args = { value };
                sbi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(SbiTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                sbi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(SbiTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        NextByte args = { 0 };
        sbi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(SbiTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x1;
        NextByte args = { 0 };
        sbi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(SbiTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x5;
        NextByte args1 = { 0x6 };
        sbi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0xff, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(SbiTest, ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        NextByte args = { 0x2 };
        sbi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(SbiTest, ShouldSetAuxCarryFlag) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xF;
        NextByte args = { 0x1 };
        sbi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(0xE, acc_reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SbiTest, ShouldNotSetAuxCarryFlag) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x10;
        NextByte args = { 0x1 };
        sbi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(0xF, acc_reg);
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SbiTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        NextByte args1 = { 0x1 };
        sbi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(7, cycles);
    }
}
