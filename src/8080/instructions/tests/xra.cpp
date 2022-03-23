#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(XraTest, ShouldOrGivenValueWithAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                xra(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg_counter ^ value, acc_reg);
            }
        }
    }

    TEST(XraTest, ShouldAlwaysClearCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                xra(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(false, flag_reg.is_carry_flag_set());
            }
        }
    }

    TEST(XraTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                xra(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(XraTest, ShouldSetSignFlagWhenAbove127AndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                xra(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(XraTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        std::uint8_t value = 0xFF;
        xra(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(XraTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x2;
        std::uint8_t value = 0xFF;
        xra(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(XraTest, ShouldAlwaysUnsetAuxCarry) {
        unsigned long cycles = 0;

        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                std::uint8_t acc_reg = acc_reg_counter;
                xra(acc_reg, value, flag_reg, cycles);
                EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
            }
        }
    }

    TEST(XraTest, ShouldUse4CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        std::uint8_t value = 0;
        Flags flag_reg;
        xra(acc_reg, value, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }

    TEST(XraTest, ShouldUse7CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        std::uint8_t value = 0;
        Flags flag_reg;
        xra(acc_reg, value, flag_reg, cycles, true);
        EXPECT_EQ(7, cycles);
    }
}
