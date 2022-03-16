#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(PushTest, ShouldPushRegistersOntoStack) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0xaa;
        std::uint8_t reg2 = 0xbb;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 });

        push(reg1, reg2, sp, memory, cycles);
        EXPECT_EQ(reg2, memory[0x1]);
        EXPECT_EQ(reg1, memory[0x2]);
        EXPECT_EQ(0x01, sp);
    }

    TEST(PushTest, ShouldPushPSWOntoStack) {
        unsigned long cycles = 0;

        Flags flag_reg;
        flag_reg.set_carry_flag();
        flag_reg.set_zero_flag();
        flag_reg.set_sign_flag();
        flag_reg.set_parity_flag();
        flag_reg.set_aux_carry_flag();
        std::uint8_t acc_reg = 0xbb;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 });

        push_psw(flag_reg, acc_reg, sp, memory, cycles);
        EXPECT_EQ(flag_reg.to_uint8_t(), memory[0x1]);
        EXPECT_EQ(acc_reg, memory[0x2]);
        EXPECT_EQ(0x01, sp);
    }

    TEST(PushTest, ShouldUse11Cycles) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 });

        push(reg1, reg2, sp, memory, cycles);
        EXPECT_EQ(11, cycles);
    }
}
