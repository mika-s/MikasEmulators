#pragma once

#include "crosscutting/memory/memory_mapped_io.h" // IWYU pragma: keep
#include <cstddef>
#include <memory>
#include <vector>

namespace emu::memory {

void dummy();

template<class A, class D>
class EmulatorMemory {
public:
    EmulatorMemory() = default;

    void add(std::vector<D> const& to_add)
    {
        const std::size_t current_size = m_memory.size();

        for (std::size_t i = current_size, j = 0; i < current_size + to_add.size(); ++i, ++j) {
            m_memory.push_back(to_add[j]);
        }
    }

    void attach_memory_mapper(std::shared_ptr<MemoryMappedIo<A, D>> memory_mapper)
    {
        m_memory_mapper = std::move(memory_mapper);
        m_memory_mapper_is_attached = true;
    }

    auto size() -> std::size_t
    {
        dummy();
        return m_memory.size();
    }

    void clear()
    {
        m_memory.clear();
    }

    /**
     * Creates a slice of the memory. Does not copy any potential memory mapper.
     *
     * @param from is the index to start from
     * @param to is the index to slice until
     * @return a new EmulatorMemory object that contains the sliced memory
     */
    auto slice(const std::size_t from, const std::size_t to) -> EmulatorMemory
    {
        std::vector<D> data;

        for (std::size_t i = from; i < to; ++i) {
            data.push_back(m_memory[i]);
        }

        EmulatorMemory sliced_memory;
        sliced_memory.add(data);

        return sliced_memory;
    }

    void write(A address, D value)
    {
        if (m_memory_mapper_is_attached) {
            m_memory_mapper->write(address, value);
        } else {
            direct_write(address, value);
        }
    }

    [[nodiscard]] auto read(A address) const -> D
    {
        if (m_memory_mapper_is_attached) {
            return m_memory_mapper->read(address);
        }

        return direct_read(address);
    }

    void direct_write(A address, D value)
    {
        m_memory[static_cast<std::vector<D>::size_type>(address)] = value;
    }

    [[nodiscard]] auto direct_read(A address) const -> D
    {
        return m_memory[static_cast<std::vector<D>::size_type>(address)];
    }

    auto begin() -> std::vector<D>::iterator
    {
        return m_memory.begin();
    }

    auto end() -> std::vector<D>::iterator
    {
        return m_memory.end();
    }

    [[nodiscard]] auto begin() const -> std::vector<D>::const_iterator
    {
        return m_memory.begin();
    }

    [[nodiscard]] auto end() const -> std::vector<D>::const_iterator
    {
        return m_memory.end();
    }

private:
    std::vector<D> m_memory;
    std::shared_ptr<MemoryMappedIo<A, D>> m_memory_mapper;
    bool m_memory_mapper_is_attached { false };
};
}
