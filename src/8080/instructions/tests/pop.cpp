#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(PopTest, ShouldPopRegisterFromStack) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0xaa;
        std::uint8_t reg2 = 0xbb;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 });

        pop(reg1, reg2, sp, memory, cycles);
        EXPECT_EQ(memory[0x03], reg2);
        EXPECT_EQ(memory[0x04], reg1);
        EXPECT_EQ(0x05, sp);
    }

    TEST(PopTest, ShouldPopPSWFromStack) {
        unsigned long cycles = 0;

        Flags flag_reg;
        std::uint8_t acc_reg = 0xaa;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0xff, 0x05, 0x06, 0x07, 0x08 });

        pop_psw(flag_reg, acc_reg, sp, memory, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
        EXPECT_EQ(true, flag_reg.is_zero_flag_set());
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
        EXPECT_EQ(true, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(true, flag_reg.is_sign_flag_set());
        EXPECT_EQ(0x05, acc_reg);
        EXPECT_EQ(0x05, sp);
    }

    TEST(PopTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;
        std::uint16_t sp = 0x03;

        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t> { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 });

        pop(reg1, reg2, sp, memory, cycles);
        EXPECT_EQ(10, cycles);
    }
}
