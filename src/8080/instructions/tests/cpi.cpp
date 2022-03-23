#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(CpiTest, ShouldCompareAccRegWithValueAndSetFlags) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                Flags flag_reg;
                NextByte args = { .farg = value };
                acc_reg = acc_reg_counter;
                cpi(acc_reg, args, flag_reg, cycles);

                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg - value) > 127, flag_reg.is_sign_flag_set());
                EXPECT_EQ(static_cast<std::uint8_t>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                EXPECT_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
            }
        }
    }

    TEST(CpiTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        NextByte args = { .farg = 0 };

        Flags flag_reg;

        cpi(acc_reg, args, flag_reg, cycles);
        EXPECT_EQ(7, cycles);
    }
}
