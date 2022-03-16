#include <cstdint>
#include <gtest/gtest.h>
#include "byte_util.h"
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(SphlTest, ShouldMoveHLIntoSP) {
        unsigned long cycles = 0;

        std::uint16_t sp = 0;
        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;

        sphl(sp, emu::util::byte::to_uint16_t(h_reg, l_reg), cycles);
        EXPECT_EQ(0x1122, sp);
    }

    TEST(SphlTest, ShouldUse5Cycles) {
        unsigned long cycles = 0;

        std::uint16_t sp = 0;
        std::uint8_t h_reg = 0x11;
        std::uint8_t l_reg = 0x22;

        sphl(sp, emu::util::byte::to_uint16_t(h_reg, l_reg), cycles);
        EXPECT_EQ(5, cycles);
    }
}
