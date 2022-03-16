#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(RarTest, ShouldRotateAccumulatorRight) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            Flags flag_reg;
            acc_reg = acc_reg_counter;
            rar(acc_reg, flag_reg, cycles);
            EXPECT_EQ(static_cast<std::uint8_t>(acc_reg_counter >> 1u), acc_reg);
        }
    }

    TEST(RarTest, ShouldSetCarryFlagIfShiftedOutOfLSB) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0b00000001;
        Flags flag_reg;
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
        rar(acc_reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(RarTest, ShouldTakeCarryFlagIntoAccountWhenShifting) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();
        rar(acc_reg, flag_reg, cycles);
        EXPECT_EQ(0b10000000, acc_reg);
    }

    TEST(RarTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 1;
        Flags flag_reg;
        flag_reg.set_carry_flag();
        rar(acc_reg, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
