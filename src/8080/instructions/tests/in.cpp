#include <gtest/gtest.h>
#include "instructions.h"

namespace emu::cpu8080::tests {
    TEST(InTest, ShouldStoreAddressedIoInAccReg) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        NextByte args = { .farg = 0x1 };
        std::vector<std::uint8_t> io = { 0, 2, 4, 6, 8, 10 };

        in(acc_reg, args, io, cycles);
        EXPECT_EQ(io[args.farg], acc_reg);
    }

    TEST(InTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        NextByte args = { .farg = 0x1 };
        std::vector<std::uint8_t> io = { 0, 1, 2, 3, 4, 5 };

        in(acc_reg, args, io, cycles);
        EXPECT_EQ(10, cycles);
    }
}
