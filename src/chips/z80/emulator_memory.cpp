#include "doctest.h"
#include "emulator_memory.h"

namespace emu::z80 {

    EmulatorMemory::EmulatorMemory()
            : m_memory_mapper_is_attached(false) {
    }

    void EmulatorMemory::add(const std::vector<u8> &to_add) {
        std::size_t current_size = m_memory.size();

        for (std::size_t i = current_size, j = 0; i < current_size + to_add.size(); ++i, ++j) {
            m_memory.push_back(to_add[j]);
        }
    }

    void EmulatorMemory::attach_memory_mapper(std::shared_ptr<MemoryMappedIo> memory_mapper) {
        m_memory_mapper = std::move(memory_mapper);
        m_memory_mapper_is_attached = true;
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
    EmulatorMemory EmulatorMemory::slice(std::size_t from, std::size_t to) {
        std::vector<u8> data;

        for (std::size_t i = from; i < to; ++i) {
            data.push_back(m_memory[i]);
        }

        EmulatorMemory sliced_memory;
        sliced_memory.add(data);

        return sliced_memory;
    }

    void EmulatorMemory::write(u16 address, u8 value) {
        if (m_memory_mapper_is_attached) {
            m_memory_mapper->write(address, value);
        } else {
            direct_write(address, value);
        }
    }

    void EmulatorMemory::direct_write(u16 address, u8 value) {
        m_memory[address] = value;
    }

    u8 EmulatorMemory::read(u16 address) const {
        if (m_memory_mapper_is_attached) {
            return m_memory_mapper->read(address);
        } else {
            return direct_read(address);
        }
    }

    u8 EmulatorMemory::direct_read(u16 address) const {
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

    TEST_CASE("Z80: EmulatorMemory") {
        SUBCASE("should index properly after one added vector") {
            EmulatorMemory memory;

            const std::vector<u8> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            memory.add(input);

            for (std::size_t i = 0; i < input.size(); ++i) {
                const u16 address = static_cast<u16>(i);
                CHECK_EQ(input[i], memory.read(address));
            }
        }

        SUBCASE("should index properly after two added vectors") {
            EmulatorMemory memory;

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
            EmulatorMemory memory;

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
            EmulatorMemory memory;

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
            EmulatorMemory memory;

            const std::vector<u8> input1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            const std::vector<u8> input2 = {11, 12, 13, 14, 15};

            memory.add(input1);
            memory.add(input2);

            CHECK_EQ(15, memory.size());
        }
    }
}
