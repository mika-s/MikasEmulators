#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(JzTest, ShouldJumpWhenZero) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jz(pc, args, flag_reg, cycles);

        EXPECT_EQ(0x2211, pc);
    }

    TEST(JzTest, ShouldNotJumpWhenNotZero) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jz(pc, args, flag_reg, cycles);

        EXPECT_EQ(0, pc);
    }

    TEST(JzTest, ShouldNotAffectAnyFlags) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        jz(pc, args, flag_reg, cycles);

        EXPECT_EQ(true, flag_reg.is_zero_flag_set());
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
        EXPECT_EQ(false, flag_reg.is_parity_flag_set());
    }

    TEST(JzTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        jz(pc, args, flag_reg, cycles);

        EXPECT_EQ(10, cycles);

        flag_reg.set_zero_flag();

        jz(pc, args, flag_reg, cycles);

        EXPECT_EQ(10, cycles);
    }
}
