#include <cstdint>
#include <gtest/gtest.h>
#include "8080/emulator_memory.h"

namespace emu::cpu8080::tests {
    using emu::cpu8080::EmulatorMemory;

    TEST(EmulatorMemoryTest, ShouldIndexProperlyAfterOneAddedVector) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        memory.add(input);

        for (std::size_t i = 0; i < input.size(); ++i) {
            EXPECT_EQ(input[i], memory[i]);
        }
    }

    TEST(EmulatorMemoryTest, ShouldIndexProperlyAfterTwoAddedVectors) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        memory.add(input);
        memory.add(input);

        for (std::size_t i = 0; i < input.size(); ++i) {
            EXPECT_EQ(input[i], memory[i]);
        }

        for (std::size_t i = input.size(), j = 0; i < 2 * input.size(); ++i, ++j) {
            EXPECT_EQ(input[j], memory[i]);
        }
    }

    TEST(EmulatorMemoryTest, ShouldIndexProperlyAfterTwoAddedVectorsAndOneLinkVector) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        const std::vector<std::uint8_t> input2 = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

        memory.add(input1);
        memory.add(input2);
        memory.add_link(7, 14);

        for (std::size_t i = 0; i < input1.size(); ++i) {
            EXPECT_EQ(input1[i], memory[i]);
        }

        for (std::size_t i = input1.size(), j = 0; i < input1.size() + input2.size(); ++i, ++j) {
            EXPECT_EQ(input2[j], memory[i]);
        }

        const std::vector<std::uint8_t> expected = { 8, 9, 10, 11, 12, 13, 14 };
        for (std::size_t i = input1.size() + input2.size(), j = 0; i < input1.size() + input2.size() + expected.size(); ++i, ++j) {
            EXPECT_EQ(expected[j], memory[i]);
        }
    }

    TEST(EmulatorMemoryTest, ShouldBePossibleToSetWithIndex) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        memory.add(input);

        memory[5] = 100;

        for (std::size_t i = 0; i < input.size(); ++i) {
            if (i == 5) {
                EXPECT_EQ(100, memory[i]);
            } else {
                EXPECT_EQ(input[i], memory[i]);
            }
        }
    }

    TEST(EmulatorMemoryTest, ShouldHaveASizeMethodThatReturnsSizeOfMemory) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        const std::vector<std::uint8_t> input2 = { 11, 12, 13, 14, 15 };

        memory.add(input1);
        memory.add(input2);
        memory.add_link(0, 5);

        EXPECT_EQ(20, memory.size());
    }

    TEST(EmulatorMemoryTest, ShouldSetPointedToIndexWhenLinkedIndex) {
        EmulatorMemory memory;

        const std::vector<std::uint8_t> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        memory.add(input);
        memory.add_link(5, 10);

        memory[13] = 100;

        for (std::size_t i = 0; i < input.size(); ++i) {
            if (i == 8 || i == 13) {
                EXPECT_EQ(100, memory[i]);
            } else {
                EXPECT_EQ(input[i], memory[i]);
            }
        }
    }
}
