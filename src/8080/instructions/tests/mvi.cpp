#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(MviTest, ShouldLoadRegistersWithValue) {
        unsigned long cycles = 0;
        std::uint8_t reg = 0;
        NextByte args = { .farg = 0 };

        mvi(reg, args, cycles);
        EXPECT_EQ(0, reg);

        args = { .farg = 0xA };
        mvi(reg, args, cycles);
        EXPECT_EQ(0xA, reg);

        mvi(reg, args, cycles);
        EXPECT_EQ(0xA, reg);

        args = { .farg = 0xFF };
        mvi(reg, args, cycles);
        EXPECT_EQ(0xFF, reg);
    }

    TEST(MviTest, ShouldLoadMemoryWithValue) {
        unsigned long cycles = 0;
        std::uint8_t reg = 0;
        NextByte args = { .farg = 0 };

        mvi(reg, args, cycles, true);
        EXPECT_EQ(0, reg);

        args = { .farg = 0xA };
        mvi(reg, args, cycles, true);
        EXPECT_EQ(0xA, reg);

        mvi(reg, args, cycles, true);
        EXPECT_EQ(0xA, reg);

        args = { .farg = 0xFF };
        mvi(reg, args, cycles, true);
        EXPECT_EQ(0xFF, reg);
    }

    TEST(MviTest, ShouldUse7CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        NextByte args = { .farg = 0x11 };

        mvi(reg, args, cycles);
        EXPECT_EQ(7, cycles);
    }

    TEST(MviTest, ShouldUse10CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;

        std::uint8_t reg = 0;
        NextByte args = { .farg = 0x21 };

        mvi(reg, args, cycles, true);
        EXPECT_EQ(10, cycles);
    }
}
