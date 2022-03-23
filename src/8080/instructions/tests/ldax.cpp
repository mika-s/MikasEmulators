#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(LdaxTest, ShouldLoadAccumulatorFromMemoryUsingAddressInArgs) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0x04;

        ldax(acc_reg, reg1, reg2, memory, cycles);
        EXPECT_EQ(memory[0x04], acc_reg);
    }

    TEST(LdaxTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0x04;

        ldax(acc_reg, reg1, reg2, memory, cycles);
        EXPECT_EQ(7, cycles);
    }
}
