#include <cstdint>
#include <gtest/gtest.h>
#include "instructions.h"
#include "flags.h"
#include "instruction_util.h"

namespace emu::cpu8080::tests {
    TEST(JpeTest, ShouldJumpWhenParityEven) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_parity_flag();   // Parity is even when the parity flag is true.

        jpe(pc, args, flag_reg, cycles);

        EXPECT_EQ(0x2211, pc);
    }

    TEST(JpeTest, ShouldNotJumpWhenNotParityEven) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_parity_flag();

        jpe(pc, args, flag_reg, cycles);

        EXPECT_EQ(0, pc);
    }

    TEST(JpeTest, ShouldNotAffectAnyFlags) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.set_parity_flag();

        jpe(pc, args, flag_reg, cycles);

        EXPECT_EQ(false, flag_reg.is_zero_flag_set());
        EXPECT_EQ(false, flag_reg.is_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_aux_carry_flag_set());
        EXPECT_EQ(false, flag_reg.is_sign_flag_set());
        EXPECT_EQ(true, flag_reg.is_parity_flag_set());
    }

    TEST(JpeTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint16_t pc = 0;
        NextWord args = { .farg = 0x11, .sarg = 0x22 };
        Flags flag_reg;
        flag_reg.clear_parity_flag();

        jpe(pc, args, flag_reg, cycles);

        EXPECT_EQ(10, cycles);

        flag_reg.set_parity_flag();

        jpe(pc, args, flag_reg, cycles);

        EXPECT_EQ(10, cycles);
    }
}
