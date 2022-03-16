#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_0) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x00;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_0(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_1) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x08;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_1(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_2) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x10;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_2(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_3) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x18;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_3(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_4) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x20;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_4(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_5) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x28;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_5(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_6) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x30;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_6(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldPushPCOntoStackAndChangeToNewPC_7) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0xac12;
        std::uint16_t expected_new_pc = 0x38;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_7(pc, sp, memory, cycles);
        EXPECT_EQ(expected_new_pc, pc);
        EXPECT_EQ(0xac, memory[0x02]);
        EXPECT_EQ(0x12, memory[0x01]);
    }

    TEST(RstTest, ShouldUse11CyclesWhenReturning) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t sp = 0x03;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        rst_0(pc, sp, memory, cycles);
        EXPECT_EQ(11, cycles);
    }
}
