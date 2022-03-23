#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(AciTest, ShouldAddGivenValueToAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                NextByte args = { value };
                aci(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter + value), acc_reg);
            }
        }
    }

    TEST(AciTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                aci(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(AciTest, ShouldSetSignFlagWhenAbove127AndNotOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                NextByte args = { 0x80 };
                aci(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(AciTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x0;
        NextByte args1 = { 0x3 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(AciTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x0;
        NextByte args1 = { 0x1 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x1, acc_reg);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(AciTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x80;
        NextByte args1 = { 0x80 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x0, acc_reg);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(AciTest, ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x1;
        NextByte args1 = { 0x2 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x3, acc_reg);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
    }

    TEST(AciTest, ShouldSetAuxCarryFlagWhenCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xF;
        NextByte args1 = { 0x1 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0x10, acc_reg);
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
    }

    TEST(AciTest, ShouldNotSetAuxCarryFlagWhenNotCarriedOutOfFourthBit) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        NextByte args1 = { 0x1 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(0xF, acc_reg);
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
    }

    TEST(AciTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0xE;
        NextByte args1 = { 0x1 };
        aci(acc_reg, args1, flag_reg, cycles);
        EXPECT_EQ(7, cycles);
    }
}
