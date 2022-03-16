#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(CnzTest, ShouldPushCurrentPCOnTheStackAndChangePCToAddressInArgsWhenZeroFlagIsUnset) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0x100f;
        std::uint16_t sp = 0x2;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        cnz(pc, sp, memory, args, flag_reg, cycles);
        EXPECT_EQ(0x0002, pc);
        EXPECT_EQ(0x0f, memory[0]);
        EXPECT_EQ(0x10, memory[1]);
    }

    TEST(CnzTest, ShouldNotDoAnythingWhenZeroFlagIsSet) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0x100f;
        std::uint16_t sp = 0x2;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        cnz(pc, sp, memory, args, flag_reg, cycles);
        EXPECT_EQ(0x100f, pc);
        EXPECT_EQ(0x00, memory[0]);
        EXPECT_EQ(0x01, memory[1]);
    }

    TEST(CnzTest, ShouldUse11CyclesWhenNotCalled) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t sp = 0x2;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        cnz(pc, sp, memory, args, flag_reg, cycles);
        EXPECT_EQ(11, cycles);
    }

    TEST(CnzTest, ShouldUse17CyclesWhenCalled) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t sp = 0x2;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        cnz(pc, sp, memory, args, flag_reg, cycles);
        EXPECT_EQ(17, cycles);
    }
}
