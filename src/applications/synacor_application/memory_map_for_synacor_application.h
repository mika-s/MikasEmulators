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

    auto read(Address address) -> RawData override;

    [[nodiscard]] auto r0() const -> Data;

    [[nodiscard]] auto r1() const -> Data;

    [[nodiscard]] auto r2() const -> Data;

    [[nodiscard]] auto r3() const -> Data;

    [[nodiscard]] auto r4() const -> Data;

    [[nodiscard]] auto r5() const -> Data;

    [[nodiscard]] auto r6() const -> Data;

    [[nodiscard]] auto r7() const -> Data;

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
