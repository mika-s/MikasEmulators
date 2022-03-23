#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(RzTest, ShouldPopPCOffTheStackWhenTheZeroFlagIsSet) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0x100f;
        std::uint16_t sp = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0xab, 0x01, 0x02, 0x03, 0x04, 0x05 });
        Flags flag_reg;
        flag_reg.set_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);
        EXPECT_EQ(0x01ab, pc);
    }

    TEST(RzTest, ShouldNotPopPCOffTheStackWhenTheZeroFlagIsUnset) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0x100f;
        std::uint16_t sp = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0xab, 0x01, 0x02, 0x03, 0x04, 0x05 });
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);
        EXPECT_EQ(0x100f, pc);
    }

    TEST(RzTest, ShouldUse5CyclesWhenNotReturning) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t sp = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });
        Flags flag_reg;

        rz(pc, sp, memory, flag_reg, cycles);
        EXPECT_EQ(5, cycles);
    }

    TEST(RzTest, ShouldUse11CyclesWhenReturning) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        std::uint16_t sp = 0;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });
        Flags flag_reg;
        flag_reg.set_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);
        EXPECT_EQ(11, cycles);
    }
}
