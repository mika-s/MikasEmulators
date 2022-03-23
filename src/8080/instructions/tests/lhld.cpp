#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(LhldTest, ShouldLoadAccumulatorFromMemoryUsingAddressInArgs) {
        unsigned long cycles = 0;

        std::uint8_t l_reg = 0xE;
        std::uint8_t h_reg = 0x42;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x04, .sarg = 0 };

        lhld(l_reg, h_reg, memory, args, cycles);
        EXPECT_EQ(memory[0x04], l_reg);
        EXPECT_EQ(memory[0x05], h_reg);
    }

    TEST(LhldTest, ShouldUse16Cycles) {
        unsigned long cycles = 0;

        std::uint8_t l_reg = 0xE;
        std::uint8_t h_reg = 0x42;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x04, .sarg = 0 };

        lhld(l_reg, h_reg, memory, args, cycles);
        EXPECT_EQ(16, cycles);
    }
}
