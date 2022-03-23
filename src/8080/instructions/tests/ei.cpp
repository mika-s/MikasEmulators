#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(EiTest, ShouldEnableInterrupts) {
        unsigned long cycles = 0;

        bool inte = false;

        ei(inte, cycles);

        EXPECT_EQ(true, inte);

        ei(inte, cycles);

        EXPECT_EQ(true, inte);
    }

    TEST(EiTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        bool inte = false;

        ei(inte, cycles);
        EXPECT_EQ(4, cycles);
    }
}
