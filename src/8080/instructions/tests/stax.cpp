#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(StaxTest, ShouldStoreAccRegInMemoryAtGivenAddress) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        std::uint8_t reg1 = 0x0;
        std::uint8_t reg2 = 0x3;

        stax(acc_reg, reg1, reg2, memory, cycles);
        EXPECT_EQ(acc_reg, memory[0x3]);
    }

    TEST(StaxTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        std::uint8_t reg1 = 0x0;
        std::uint8_t reg2 = 0x0;

        stax(acc_reg, reg1, reg2, memory, cycles);
        EXPECT_EQ(7, cycles);
    }
}
