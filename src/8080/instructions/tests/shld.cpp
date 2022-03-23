#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(ShldTest, ShouldLoadMemoryWithLAndHRegisters) {
        unsigned long cycles = 0;

        std::uint8_t l_reg = 0x22;
        std::uint8_t h_reg = 0x11;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0xff, 0xaa, 0xbb, 0xcc, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };

        shld(l_reg, h_reg, memory, args, cycles);
        EXPECT_EQ(l_reg, memory[0x02]);
        EXPECT_EQ(h_reg, memory[0x03]);
    }

    TEST(ShldTest, ShouldUse16Cycles) {
        unsigned long cycles = 0;

        std::uint8_t l_reg = 0x22;
        std::uint8_t h_reg = 0x11;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };

        shld(l_reg, h_reg, memory, args, cycles);
        EXPECT_EQ(16, cycles);
    }
}
