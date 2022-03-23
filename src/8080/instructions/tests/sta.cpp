#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(StaTest, ShouldStoreAccRegInMemoryAtGivenAddress) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0x45;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x3, .sarg = 0x0 };

        sta(acc_reg, memory, args, cycles);
        EXPECT_EQ(acc_reg, memory[0x3]);
    }

    TEST(StaTest, ShouldUse13Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x0, .sarg = 0x0 };

        sta(acc_reg, memory, args, cycles);
        EXPECT_EQ(13, cycles);
    }
}
