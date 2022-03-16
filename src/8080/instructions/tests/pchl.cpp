#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(PchlTest, ShouldLoadAddressIntoPC) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0x1111;
        std::uint16_t address = 0x432a;

        pchl(pc, address, cycles);
        EXPECT_EQ(address, pc);
    }

    TEST(PchlTest, ShouldUse5Cycles) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t address = 0;

        pchl(pc, address, cycles);
        EXPECT_EQ(5, cycles);
    }
}
