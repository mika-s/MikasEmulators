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
using emu::synacor::RawData;
using emu::util::byte::low_nibble;

class MemoryMapForSynacorApplication : public MemoryMappedIo<Address, RawData> {
public:
    explicit MemoryMapForSynacorApplication(EmulatorMemory<Address, RawData>& memory);

    void write(Address address, RawData value) override;

    RawData read(Address address) override;

    [[nodiscard]] Data r0() const;

    [[nodiscard]] Data r1() const;

    [[nodiscard]] Data r2() const;

    [[nodiscard]] Data r3() const;

    [[nodiscard]] Data r4() const;

    [[nodiscard]] Data r5() const;

    [[nodiscard]] Data r6() const;

    [[nodiscard]] Data r7() const;

private:
    EmulatorMemory<Address, RawData>& m_memory;
    Data m_r0 { 0 };
    Data m_r1 { 0 };
    Data m_r2 { 0 };
    Data m_r3 { 0 };
    Data m_r4 { 0 };
    Data m_r5 { 0 };
    Data m_r6 { 0 };
    Data m_r7 { 0 };
};
}
