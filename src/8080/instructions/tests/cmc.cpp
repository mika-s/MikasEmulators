#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(CmcTest, ShouldToggleCarryFlag) {
        unsigned long cycles = 0;

        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_carry_flag_set());

        cmc(flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());

        cmc(flag_reg, cycles);
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());
    }

    TEST(CmcTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        Flags flag_reg;

        cmc(flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
