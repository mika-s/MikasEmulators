#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(DcxTest, ShouldDecreaseRegisterPair) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = UINT8_MAX;
        std::uint8_t reg2 = UINT8_MAX;
        std::uint8_t expected_reg1 = UINT8_MAX;
        std::uint8_t expected_reg2;

        for (int i = UINT16_MAX; i > UINT16_MAX; --i) {
            if (reg2 == 0 && i != 0) {
                --expected_reg1;
            }

            expected_reg2 = i - 1;

            dcx(reg1, reg2, cycles);

            EXPECT_EQ(expected_reg1, reg1);
            EXPECT_EQ(expected_reg2, reg2);
        }
    }

    TEST(InxTest, ShouldDecreaseSP) {
        unsigned long cycles = 0;

        std::uint16_t sp = UINT16_MAX;

        for (std::uint16_t expected_sp = UINT16_MAX; expected_sp > 0; --expected_sp) {
            sp = expected_sp;
            dcx_sp(sp, cycles);

            EXPECT_EQ(expected_sp - 1, sp);
        }
    }

    TEST(DcxTest, ShouldUse5Cycles) {
        unsigned long cycles = 0;
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;

        dcx(reg1, reg2, cycles);
        EXPECT_EQ(5, cycles);

        std::uint16_t sp = 0;

        dcx_sp(sp, cycles);
        EXPECT_EQ(5, cycles);
    }
}
