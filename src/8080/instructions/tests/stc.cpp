#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(StcTest, ShouldSetCarryFlag) {
        unsigned long cycles = 0;

        Flags flag_reg;

        EXPECT_EQ(false, flag_reg.is_carry_flag_set());

        stc(flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
        EXPECT_EQ(false, flag_reg.is_zero_flag_set());
    }

    TEST(StcTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        Flags flag_reg;

        stc(flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
