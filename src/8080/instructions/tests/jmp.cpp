#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(JmpTest, ShouldJump) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };

        jmp(pc, args, cycles);

        EXPECT_EQ(0x2211, pc);
    }

    TEST(JmpTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };

        jmp(pc, args, cycles);

        EXPECT_EQ(10, cycles);
    }
}
