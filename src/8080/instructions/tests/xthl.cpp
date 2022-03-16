#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(XthlTest, ShouldExchangeHLWithTopOfStack) {
        unsigned long cycles = 0;

        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;
        std::uint8_t sp0 = 0x33;
        std::uint8_t sp1 = 0x44;

        xthl(h_reg, l_reg, sp0, sp1, cycles);
        EXPECT_EQ(0x44, h_reg);
        EXPECT_EQ(0x33, l_reg);
        EXPECT_EQ(0x22, sp0);
        EXPECT_EQ(0x11, sp1);
    }

    TEST(XthlTest, ShouldUse18Cycles) {
        unsigned long cycles = 0;

        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;
        std::uint8_t sp0 = 0;
        std::uint8_t sp1 = 0;

        xthl(h_reg, l_reg, sp0, sp1, cycles);
        EXPECT_EQ(18, cycles);
    }
}
