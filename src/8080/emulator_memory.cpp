#include <stdexcept>
#include <sstream>
#include "doctest.h"
#include "8080/emulator_memory.h"

namespace emu::cpu8080 {

    void EmulatorMemory::add(const std::vector<std::uint8_t> &to_add) {
        std::size_t current_size = m_real_memory.size();

        for (unsigned long i = current_size, j = 0; i < current_size + to_add.size(); ++i, ++j) {
            m_real_memory.push_back(to_add[j]);
            m_indices.push_back(i);
        }
    }

    void EmulatorMemory::add_link(std::uint16_t from, std::uint16_t to) {
        if (from > m_real_memory.size()) {
            throw std::runtime_error("from is larger than the memory size");
        } else if (to > m_real_memory.size()) {
            throw std::runtime_error("to is larger than the memory size");
        } else if (from > to) {
            throw std::runtime_error("from is larger than to");
        }

        for (uint16_t i = from; i < to; ++i) {
            m_indices.push_back(i);
        }
    }

    std::size_t EmulatorMemory::size() {
        return m_indices.size();
    }

    /**
     * Creates a slice of the memory. Follows links, but does not create new links.
     *
     * @param from is the index to start from
     * @param to is the index to slice until
     * @return a new EmulatorMemory object that contains the sliced memory
     */
    EmulatorMemory EmulatorMemory::slice(int from, int to) {
        std::vector<std::uint8_t> data;

        for (int i = from; i < to; ++i) {
            data.push_back(m_real_memory[m_indices[i]]);
        }

        EmulatorMemory sliced_memory;
        sliced_memory.add(data);

        return sliced_memory;
    }

    std::uint8_t &EmulatorMemory::operator[](std::size_t d) {
        if (d > m_indices.size()) {
            std::stringstream ss;
            ss << "index " << d << " is going past the memory end";
            throw std::runtime_error(ss.str());
        }

        return m_real_memory[m_indices[d]];
    }

    const std::uint8_t &EmulatorMemory::operator[](std::size_t d) const {
        if (d > m_indices.size()) {
            std::stringstream ss;
            ss << "index " << d << " is going past the memory end";
            throw std::runtime_error(ss.str());
        }

        return m_real_memory[m_indices[d]];
    }

    std::vector<std::uint8_t>::iterator EmulatorMemory::begin() {
        return m_real_memory.begin();
    }

    std::vector<std::uint8_t>::iterator EmulatorMemory::end() {
        return m_real_memory.end();
    }

    std::vector<std::uint8_t>::const_iterator EmulatorMemory::begin() const {
        return m_real_memory.begin();
    }

    std::vector<std::uint8_t>::const_iterator EmulatorMemory::end() const {
        return m_real_memory.end();
    }

    TEST_CASE("8080: EmulatorMemory") {
        SUBCASE("should index properly after one added vector") {
            EmulatorMemory memory;

            const std::vector<std::uint8_t> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                CHECK_EQ(input[i], memory[i]);
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory memory;

            const std::vector<std::uint8_t> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);
            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                CHECK_EQ(input[i], memory[i]);
            }

            for (std::size_t i = input.size(), j = 0; i < 2 * input.size(); ++i, ++j) {
                CHECK_EQ(input[j], memory[i]);
            }
        }

        SUBCASE("should index properly after two added vectors and one link vector") {
            EmulatorMemory memory;

            const std::vector<std::uint8_t> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<std::uint8_t> input2 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

            memory.add(input1);
            memory.add(input2);
            memory.add_link(7, 14);

            for (std::size_t i = 0; i < input1.size(); ++i) {
                CHECK_EQ(input1[i], memory[i]);
            }

            for (std::size_t i = input1.size(), j = 0; i < input1.size() + input2.size(); ++i, ++j) {
                CHECK_EQ(input2[j], memory[i]);
            }

            const std::vector<std::uint8_t> expected = {8, 9, 10, 11, 12, 13, 14};
            for (std::size_t i = input1.size() + input2.size(), j = 0;
                 i < input1.size() + input2.size() + expected.size(); ++i, ++j) {
                CHECK_EQ(expected[j], memory[i]);
            }
        }

        SUBCASE("should be possible to set with index") {
            EmulatorMemory memory;

            const std::vector<std::uint8_t> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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

            const std::vector<std::uint8_t> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<std::uint8_t> input2 = {11, 12, 13, 14, 15};

            memory.add(input1);
            memory.add(input2);
            memory.add_link(0, 5);

            CHECK_EQ(20, memory.size());
        }

        SUBCASE("should set pointed to index when linked index") {
            EmulatorMemory memory;

            const std::vector<std::uint8_t> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);
            memory.add_link(5, 10);

            memory[13] = 100;

            for (std::size_t i = 0; i < input.size(); ++i) {
                if (i == 8 || i == 13) {
                    CHECK_EQ(100, memory[i]);
                } else {
                    CHECK_EQ(input[i], memory[i]);
                }
            }
        }
    }
}
