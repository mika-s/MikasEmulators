#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(LxiTest, ShouldLoadImmidiateIntoRegisterPair) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0xE;
        std::uint8_t reg2 = 0;
        NextWord args = { .farg = 0x12, .sarg = 0x3a };

        lxi(reg1, reg2, args, cycles);
        EXPECT_EQ(args.sarg, reg1);
        EXPECT_EQ(args.farg, reg2);
    }

    TEST(LxiTest, ShouldLoadImmidiateIntoSP) {
        unsigned long cycles = 0;

        std::uint16_t sp = 0xE;
        NextWord args = { .farg = 0x12, .sarg = 0x3a };

        lxi_sp(sp, args, cycles);
        EXPECT_EQ(0x3a12, sp);
    }

    TEST(LxiTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0xE;
        std::uint8_t reg2 = 0;
        NextWord args = { .farg = 0x12, .sarg = 0x3a };

        lxi(reg1, reg2, args, cycles);
        EXPECT_EQ(10, cycles);
    }
}
