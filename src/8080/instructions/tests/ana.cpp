#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080::tests {
    TEST(AnaTest, ShouldAndGivenValueWithAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                ana(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg_counter & value, acc_reg);
            }
        }
    }

    TEST(AnaTest, ShouldAlwaysClearCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                ana(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(false, flag_reg.is_carry_flag_set());
            }
        }
    }

    TEST(AnaTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                ana(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(AnaTest, ShouldSetSignFlagWhenAbove127AndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                ana(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(AnaTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        std::uint8_t value = 0xFF;
        ana(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(AnaTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x2;
        std::uint8_t value = 0xFF;
        ana(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(AnaTest, ShouldSetAuxCarryWhenBitwiseOredThirdBitIsSet) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                ana(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(emu::util::byte::is_bit_set(acc_reg_counter | value, 3), flag_reg.is_aux_carry_flag_set());
            }
        }
    }

    TEST(AnaTest, ShouldUse4CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        std::uint8_t value = 0;
        Flags flag_reg;
        ana(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }

    TEST(AnaTest, ShouldUse7CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        std::uint8_t value = 0;
        Flags flag_reg;
        ana(acc_reg, value, flag_reg, cycles, true);
        EXPECT_EQ(7, cycles);
    }
}
