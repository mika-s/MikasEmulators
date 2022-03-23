#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(XchgTest, ShouldExchangeHLWithDE) {
        unsigned long cycles = 0;

        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;
        std::uint8_t d_reg = 0x33;
        std::uint8_t e_reg = 0x44;

        xchg(h_reg, l_reg, d_reg, e_reg, cycles);
        EXPECT_EQ(0x33, h_reg);
        EXPECT_EQ(0x44, l_reg);
        EXPECT_EQ(0x11, d_reg);
        EXPECT_EQ(0x22, e_reg);
    }

    TEST(XchgTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;
        std::uint8_t d_reg = 0;
        std::uint8_t e_reg = 0;

        xchg(h_reg, l_reg, d_reg, e_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
