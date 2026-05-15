#include "memory_map_for_synacor_application.h"
#include "crosscutting/memory/emulator_memory.h"

namespace emu::applications::synacor {

MemoryMapForSynacorApplication::MemoryMapForSynacorApplication(EmulatorMemory<Address, RawData>& memory)
    : m_memory(memory)
{
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMapForSynacorApplication::write(const Address address, const RawData value)
{
    if (address <= Address(32767)) {
        m_memory.direct_write(address, value);
    } else if (address == Address(32768)) {
        m_r0 = Data(value.underlying());
    } else if (address == Address(32769)) {
        m_r1 = Data(value.underlying());
    } else if (address == Address(32770)) {
        m_r2 = Data(value.underlying());
    } else if (address == Address(32771)) {
        m_r3 = Data(value.underlying());
    } else if (address == Address(32772)) {
        m_r4 = Data(value.underlying());
    } else if (address == Address(32773)) {
        m_r5 = Data(value.underlying());
    } else if (address == Address(32774)) {
        m_r6 = Data(value.underlying());
    } else if (address == Address(32775)) {
        m_r7 = Data(value.underlying());
    }
}

/**
 * Called when the CPU reads from memory
 *
 * @param address is the address in memory to read from
 * @return the value in memory at the given address
 */
auto MemoryMapForSynacorApplication::read(const Address address) -> RawData
{
    if (address <= Address(32767)) {
        return m_memory.direct_read(address);
    }
    if (address == Address(32768)) {
        return RawData(m_r0.underlying());
    }
    if (address == Address(32769)) {
        return RawData(m_r1.underlying());
    }
    if (address == Address(32770)) {
        return RawData(m_r2.underlying());
    }
    if (address == Address(32771)) {
        return RawData(m_r3.underlying());
    }
    if (address == Address(32772)) {
        return RawData(m_r4.underlying());
    }
    if (address == Address(32773)) {
        return RawData(m_r5.underlying());
    }
    if (address == Address(32774)) {
        return RawData(m_r6.underlying());
    }
    if (address == Address(32775)) {
        return RawData(m_r7.underlying());
    }

    throw std::runtime_error("Invalid address");
}

auto MemoryMapForSynacorApplication::r0() const -> Data
{
    return m_r0;
}

auto MemoryMapForSynacorApplication::r1() const -> Data
{
    return m_r1;
}

auto MemoryMapForSynacorApplication::r2() const -> Data
{
    return m_r2;
}

auto MemoryMapForSynacorApplication::r3() const -> Data
{
    return m_r3;
}

auto MemoryMapForSynacorApplication::r4() const -> Data
{
    return m_r4;
}

auto MemoryMapForSynacorApplication::r5() const -> Data
{
    return m_r5;
}

auto MemoryMapForSynacorApplication::r6() const -> Data
{
    return m_r6;
}

auto MemoryMapForSynacorApplication::r7() const -> Data
{
    return m_r7;
}

}
