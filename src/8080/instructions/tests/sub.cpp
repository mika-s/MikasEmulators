#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(SubTest, ShouldSubGivenValueFromAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sub(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter - value), acc_reg);
            }
        }
    }

    TEST(SubTest, ShouldSubGivenValueToAccumulatorWithoutTakingCarryIntoAccount) {
        unsigned long cycles = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        std::uint8_t acc_reg = 0xab;
        sub(acc_reg, 0x1 , flag_reg, cycles);
        EXPECT_EQ(0xaa, acc_reg);
    }

    TEST(SubTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sub(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(SubTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sub(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(SubTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        sub(acc_reg, 0x0, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(SubTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x1;
        sub(acc_reg, 0x0, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(SubTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x80;
        sub(acc_reg, 0x81, flag_reg, cycles);
        EXPECT_EQ(0xff, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(SubTest, ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x2;
        sub(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(SubTest, ShouldSetAuxCarryFlagWhenCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sub(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0xD, acc_reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SubTest, ShouldNotSetAuxCarryFlagWhenNotCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x10;
        sub(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0xF, acc_reg);
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SubTest, ShouldUse4CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sub(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }

    TEST(SubTest, ShouldUse7CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sub(acc_reg, 0x1, flag_reg, cycles, true);
        EXPECT_EQ(7, cycles);
    }
}
