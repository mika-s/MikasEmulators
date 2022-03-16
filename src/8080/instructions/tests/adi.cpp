#include <cstdint>
#include <gtest/gtest.h>
#include "flags.h"
#include "instructions.h"
#include "next_byte.h"

namespace emu::cpu8080::tests {
    TEST(AdiTest, ShouldAddGivenValueToAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                NextByte args = { value };
                adi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter + value), acc_reg);
            }
        }
    }

    TEST(AdiTest, ShouldAddGivenValueToAccumulatorWithoutTakingCarryIntoAccount) {
        unsigned long cycles = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();

        std::uint8_t acc_reg = 0;
        NextByte args1 = { 0xab };
        adi(acc_reg, args1 , flag_reg, cycles);
        EXPECT_EQ(0xab, acc_reg);
    }

    TEST(AdiTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                adi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(AdiTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                adi(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(AdiTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x0;
        NextByte args1 = { 0x3 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(AdiTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x0;
        NextByte args1 = { 0x1 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(AdiTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x80;
        NextByte args1 = { 0x80 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x0, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(AdiTest, ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x1;
        NextByte args1 = { 0x2 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(AdiTest, ShouldSetAuxCarryFlagWhenCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xF;
        NextByte args1 = { 0x1 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x10, acc_reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(AdiTest, ShouldNotSetAuxCarryFlagWhenNotCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        NextByte args1 = { 0x1 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0xF, acc_reg);
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
    }

    TEST(AdiTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        NextByte args1 = { 0x1 };
        adi(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(7, cycles);
    }
}
