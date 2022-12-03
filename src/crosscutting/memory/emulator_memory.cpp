#include "emulator_memory.h"
#include "doctest.h"
#include "crosscutting/typedefs.h"

namespace emu::memory {

    void dummy() {
        // To force the cpp file to be compiled and the tests to be registered.
    }

    TEST_CASE("crosscutting: EmulatorMemory") {
        SUBCASE("should index properly after one added vector") {
            EmulatorMemory<u16, u8> memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input[i], memory.read(address));
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory<u16, u8> memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);
            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input[i], memory.read(address));
            }

            for (std::size_t i = input.size(), j = 0; i < 2 * input.size(); ++i, ++j) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input[j], memory.read(address));
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory<u16, u8> memory;

            const std::vector<u8> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<u8> input2 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

            memory.add(input1);
            memory.add(input2);

            for (std::size_t i = 0; i < input1.size(); ++i) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input1[i], memory.read(address));
            }

            for (std::size_t i = input1.size(), j = 0; i < input1.size() + input2.size(); ++i, ++j) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input2[j], memory.read(address));
            }
        }

        SUBCASE("should be possible to set with index") {
            EmulatorMemory<u16, u8> memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            memory.write(5, 100);

            for (std::size_t i = 0; i < input.size(); ++i) {
                const u16 address = static_cast<u16>(i);
                if (address == 5) {
                    CHECK_EQ(100, memory.read(address));
                } else {
                    CHECK_EQ(input[i], memory.read(address));
                }
            }
        }

        SUBCASE("should have a size method that returns size of memory") {
            EmulatorMemory<u16, u8> memory;

            const std::vector<u8> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<u8> input2 = {11, 12, 13, 14, 15};

            memory.add(input1);
            memory.add(input2);

            CHECK_EQ(15, memory.size());
        }
    }
}
