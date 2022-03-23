#include <gtest/gtest.h>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080::tests {
    TEST(HltTest, ShouldStopProgram) {
        unsigned long cycles = 0;

        bool stopped = false;

        hlt(stopped, cycles);

        EXPECT_EQ(true, stopped);

        hlt(stopped, cycles);

        EXPECT_EQ(true, stopped);
    }

    TEST(HltTest, ShouldUse7Cycles) {
        unsigned long cycles = 0;

        bool stopped = false;

        hlt(stopped, cycles);
        EXPECT_EQ(7, cycles);
    }
}
