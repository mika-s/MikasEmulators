#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "8080/instructions/instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(SbbTest, ShouldSubGivenValueFromAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sbb(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter - value), acc_reg);
            }
        }
    }

    TEST(SbbTest, ShouldSubGivenValueToAccumulatorWithTakingCarryIntoAccount) {
        unsigned long cycles = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        std::uint8_t acc_reg = 0xab;
        sbb(acc_reg, 0x1 , flag_reg, cycles);
        EXPECT_EQ(0xa9, acc_reg);
    }

    TEST(SbbTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sbb(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(SbbTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                sbb(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(SbbTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        sbb(acc_reg, 0x0, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(SbbTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x1;
        sbb(acc_reg, 0x0, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(SbbTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x80;
        sbb(acc_reg, 0x81, flag_reg, cycles);
        EXPECT_EQ(0xff, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(SbbTest, ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x2;
        sbb(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(SbbTest, ShouldSetAuxCarryFlagWhenCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sbb(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0xD, acc_reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SbbTest, ShouldNotSetAuxCarryFlagWhenNotCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x10;
        sbb(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(0xF, acc_reg);
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
    }

    TEST(SbbTest, ShouldUse4CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sbb(acc_reg, 0x1, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }

    TEST(SbbTest, ShouldUse7CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        sbb(acc_reg, 0x1, flag_reg, cycles, true);
        EXPECT_EQ(7, cycles);
    }
}
