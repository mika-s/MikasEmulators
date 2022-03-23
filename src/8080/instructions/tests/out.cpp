#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(OutTest, ShouldStoreAccRegInAddressedIo) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 100;
        NextByte args = { .farg = 0x1 };
        std::vector<std::uint8_t> io = { 0, 2, 4, 6, 8, 10 };

        out(acc_reg, args, io, cycles);
        EXPECT_EQ(acc_reg, io[args.farg]);
    }

    TEST(OutTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;

        std::uint8_t acc_reg = 0;
        NextByte args = { .farg = 0x1 };
        std::vector<std::uint8_t> io = { 0, 1, 2, 3, 4, 5 };

        out(acc_reg, args, io, cycles);
        EXPECT_EQ(10, cycles);
    }
}
