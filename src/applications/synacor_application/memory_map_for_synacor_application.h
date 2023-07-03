#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <cstddef>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::applications::synacor {

using emu::memory::EmulatorMemory;
using emu::memory::MemoryMappedIo;
using emu::synacor::Address;
using emu::synacor::Data;
using emu::util::byte::low_nibble;

class MemoryMapForSynacorApplication : public MemoryMappedIo<Address, Data> {
public:
    explicit MemoryMapForSynacorApplication(EmulatorMemory<Address, Data>& memory);

    void write(Address address, Data value) override;

    Data read(Address address) override;

private:
    EmulatorMemory<Address, Data>& m_memory;
};
}
