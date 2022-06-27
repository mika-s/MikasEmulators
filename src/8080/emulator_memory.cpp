#include <stdexcept>
#include "doctest.h"
#include "8080/emulator_memory.h"

namespace emu::i8080 {

    void EmulatorMemory::add(const std::vector<u8> &to_add) {
        std::size_t current_size = m_memory.size();

        for (unsigned long i = current_size, j = 0; i < current_size + to_add.size(); ++i, ++j) {
            m_memory.push_back(to_add[j]);
        }
    }

    std::size_t EmulatorMemory::size() {
        return m_memory.size();
    }

    /**
     * Creates a slice of the memory.
     *
     * @param from is the index to start from
     * @param to is the index to slice until
     * @return a new EmulatorMemory object that contains the sliced memory
     */
    EmulatorMemory EmulatorMemory::slice(int from, int to) {
        std::vector<u8> data;

        for (int i = from; i < to; ++i) {
            data.push_back(m_memory[i]);
        }

        EmulatorMemory sliced_memory;
        sliced_memory.add(data);

        return sliced_memory;
    }

    u8 &EmulatorMemory::operator[](std::size_t address) {
        return m_memory[address];
    }

    const u8 &EmulatorMemory::operator[](std::size_t address) const {
        return m_memory[address];
    }

    std::vector<u8>::iterator EmulatorMemory::begin() {
        return m_memory.begin();
    }

    std::vector<u8>::iterator EmulatorMemory::end() {
        return m_memory.end();
    }

    std::vector<u8>::const_iterator EmulatorMemory::begin() const {
        return m_memory.begin();
    }

    std::vector<u8>::const_iterator EmulatorMemory::end() const {
        return m_memory.end();
    }

    TEST_CASE("8080: EmulatorMemory") {
        SUBCASE("should index properly after one added vector") {
            EmulatorMemory memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                CHECK_EQ(input[i], memory[i]);
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);
            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                CHECK_EQ(input[i], memory[i]);
            }

            for (std::size_t i = input.size(), j = 0; i < 2 * input.size(); ++i, ++j) {
                CHECK_EQ(input[j], memory[i]);
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory memory;

            const std::vector<u8> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<u8> input2 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

            memory.add(input1);
            memory.add(input2);

            for (std::size_t i = 0; i < input1.size(); ++i) {
                CHECK_EQ(input1[i], memory[i]);
            }

            for (std::size_t i = input1.size(), j = 0; i < input1.size() + input2.size(); ++i, ++j) {
                CHECK_EQ(input2[j], memory[i]);
            }
        }

        SUBCASE("should be possible to set with index") {
            EmulatorMemory memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            memory[5] = 100;

            for (std::size_t i = 0; i < input.size(); ++i) {
                if (i == 5) {
                    CHECK_EQ(100, memory[i]);
                } else {
                    CHECK_EQ(input[i], memory[i]);
                }
            }
        }

        SUBCASE("should have a size method that returns size of memory") {
            EmulatorMemory memory;

            const std::vector<u8> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<u8> input2 = {11, 12, 13, 14, 15};

            memory.add(input1);
            memory.add(input2);

            CHECK_EQ(15, memory.size());
        }
    }
}
