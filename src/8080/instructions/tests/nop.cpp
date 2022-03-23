#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(NopTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        nop(cycles);
        EXPECT_EQ(4, cycles);
    }
}
