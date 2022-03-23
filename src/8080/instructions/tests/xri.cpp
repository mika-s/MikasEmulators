#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(XriTest, ShouldXorGivenValueWithAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                xri(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg_counter ^ value, acc_reg);
            }
        }
    }

    TEST(XriTest, ShouldAlwaysClearCarryFlag) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                xri(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(false, flag_reg.is_carry_flag_set());
            }
        }
    }

    TEST(XriTest, ShouldSetZeroFlagWhenZeroAndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                xri(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
            }
        }
    }

    TEST(XriTest, ShouldSetSignFlagWhenAbove127AndNotSetOtherwise) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                xri(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
            }
        }
    }

    TEST(XriTest, ShouldSetParityFlagWhenEvenParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x3;
        NextByte args = { 0xFF };
        xri(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(XriTest, ShouldNotSetParityFlagWhenOddParity) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t acc_reg = 0x2;
        NextByte args = { 0xFF };
        xri(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(XriTest, ShouldAlwaysUnsetAuxCarry) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { value };
                acc_reg = acc_reg_counter;
                xri(acc_reg, args, flag_reg, cycles);
                EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
            }
        }
    }

    TEST(XriTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        NextByte args = { 0 };
        Flags flag_reg;
        xri(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(7, cycles);
    }
}
