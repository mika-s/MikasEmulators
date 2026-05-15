#pragma once

#include "crosscutting/typedefs.h"

namespace emu::memory {

template<class A, class D>
class MemoryMappedIo {
public:
    virtual ~MemoryMappedIo() = default;

    virtual auto read(A address) -> D = 0;

    virtual void write(A address, D value) = 0;
};
}
