#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(InxTest, ShouldIncreaseRegisterPair) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;
        std::uint8_t expected_reg1 = 0;
        std::uint8_t expected_reg2;

        for (int i = 0; i < UINT16_MAX; ++i) {
            inx(reg1, reg2, cycles);

            if (reg2 % (UINT8_MAX + 1) == 0 && i != 0) {
                ++expected_reg1;
            }

            expected_reg2 = i + 1;

            EXPECT_EQ(expected_reg1, reg1);
            EXPECT_EQ(expected_reg2, reg2);
        }
    }

    TEST(InxTest, ShouldIncreaseSP) {
        unsigned long cycles = 0;

        std::uint16_t sp = 0;

        for (std::uint16_t expected_sp = 0; expected_sp < UINT16_MAX; ++expected_sp) {
            sp = expected_sp;
            inx_sp(sp, cycles);

            EXPECT_EQ(expected_sp + 1, sp);
        }
    }

    TEST(InxTest, ShouldUse5Cycles) {
        unsigned long cycles = 0;
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;

        inx(reg1, reg2, cycles);
        EXPECT_EQ(5, cycles);

        std::uint16_t sp = 0;

        inx_sp(sp, cycles);
        EXPECT_EQ(5, cycles);
    }
}
