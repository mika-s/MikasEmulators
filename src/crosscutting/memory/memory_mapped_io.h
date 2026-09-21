#pragma once

#include "crosscutting/typedefs.h"

namespace emu::memory {

template<class A, class D>
class MemoryMappedIo {
public:
    virtual ~MemoryMappedIo() = default;

    MemoryMappedIo(const MemoryMappedIo&) = delete;
    auto operator=(const MemoryMappedIo&) -> MemoryMappedIo& = delete;
    MemoryMappedIo(MemoryMappedIo&&) = delete;
    auto operator=(MemoryMappedIo&&) -> MemoryMappedIo& = delete;

    virtual auto read(A address) -> D = 0;

    virtual void write(A address, D value) = 0;

protected:
    MemoryMappedIo() = default;
};
}
