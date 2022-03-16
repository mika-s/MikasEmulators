#include <cstdint>
#include <gtest/gtest.h>
#include "byte_util.h"
#include "instructions.h"
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(RlcTest, ShouldRotateAccumulatorLeft) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            Flags flag_reg;
            acc_reg = acc_reg_counter;
            const bool cy = emu::util::byte::is_bit_set(acc_reg, HIGH_BIT);

            rlc(acc_reg, flag_reg, cycles);

            std::uint8_t expected = acc_reg_counter << 1u;
            if (cy) {
                emu::util::byte::set_bit(expected, LOW_BIT);
            }

            EXPECT_EQ(expected, acc_reg);
        }
    }

    TEST(RlcTest, ShouldSetCarryFlagIfShiftedOutOfMSB) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0b10000000;
        Flags flag_reg;
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
        rlc(acc_reg, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(RlcTest, ShouldNotTakeCarryFlagIntoAccountWhenShifting) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        Flags flag_reg;
        flag_reg.set_carry_flag();
        rlc(acc_reg, flag_reg, cycles);
        EXPECT_EQ(0, acc_reg);
    }

    TEST(RlcTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 1;
        Flags flag_reg;
        flag_reg.set_carry_flag();
        rlc(acc_reg, flag_reg, cycles);
        EXPECT_EQ(4, cycles);
    }
}
