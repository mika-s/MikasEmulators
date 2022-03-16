#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(MovTest, ShouldLoadRegistersWithValue) {
        unsigned long cycles = 0;
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;

        mov(reg1, reg2, cycles);
        EXPECT_EQ(0, reg1);

        reg2 = 0xA;
        mov(reg1, reg2, cycles);
        EXPECT_EQ(0xA, reg1);

        reg2 = 0xFF;
        mov(reg1, reg2, cycles);
        EXPECT_EQ(0xFF, reg1);
    }

    TEST(MovTest, ShouldLoadMemoryWithValue) {
        unsigned long cycles = 0;
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;

        mov(reg1, reg2, cycles, true);
        EXPECT_EQ(0, reg1);

        reg2 = 0xA;
        mov(reg1, reg2, cycles, true);
        EXPECT_EQ(0xA, reg1);

        reg2 = 0xFF;
        mov(reg1, reg2, cycles, true);
        EXPECT_EQ(0xFF, reg1);
    }

    TEST(MovTest, ShouldUse5CyclesIfMemoryIsNotInvolved) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0x11;

        mov(reg1, reg2, cycles);
        EXPECT_EQ(5, cycles);
    }

    TEST(MovTest, ShouldUse7CyclesIfMemoryIsInvolved) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0x11;

        mov(reg1, reg2, cycles, true);
        EXPECT_EQ(7, cycles);
    }
}
