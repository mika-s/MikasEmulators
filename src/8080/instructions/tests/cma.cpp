#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(CmaTest, ShouldComplementTheAccumulator) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                acc_reg = acc_reg_counter;
                cma(acc_reg, cycles);
                EXPECT_EQ(static_cast<std::uint8_t>(~acc_reg_counter), acc_reg);
            }
        }
    }

    TEST(CmaTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;

        cma(acc_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
