#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(DiTest, ShouldDisableInterrupts) {
        unsigned long cycles = 0;

        bool inte = true;

        di(inte, cycles);

        EXPECT_EQ(false, inte);

        di(inte, cycles);

        EXPECT_EQ(false, inte);
    }

    TEST(DiTest, ShouldUse4Cycles) {
        unsigned long cycles = 0;

        bool inte = false;

        di(inte, cycles);
        EXPECT_EQ(4, cycles);
    }
}
