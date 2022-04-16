#include <stdexcept>
#include <sstream>
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
}
