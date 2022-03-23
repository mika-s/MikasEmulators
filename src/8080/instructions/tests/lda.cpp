#include <cstdint>
#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(LdaTest, ShouldLoadAccumulatorFromMemoryUsingAddressInArgs) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x04, .sarg = 0 };

        lda(acc_reg, memory, args, cycles);
        EXPECT_EQ(memory[0x04], acc_reg);
    }

    TEST(LdaTest, ShouldUse13Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0xE;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x04, .sarg = 0 };

        lda(acc_reg, memory, args, cycles);
        EXPECT_EQ(13, cycles);
    }
}
