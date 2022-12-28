#include "memory_map_for_zxspectrum_48k.h"
#include "crosscutting/memory/emulator_memory.h"

namespace emu::applications::zxspectrum_48k {

MemoryMapForZxSpectrum48k::MemoryMapForZxSpectrum48k(EmulatorMemory<u16, u8>& memory)
    : m_memory(memory)
{
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMapForZxSpectrum48k::write(u16 address, u8 value)
{
    if (address <= s_address_rom_end) {
    } else if (address <= s_address_ram_end) {
        m_memory.direct_write(address, value);
    } else {
    }
}

/**
 * Called when the CPU reads from memory
 *
 * @param address is the address in memory to read from
 * @return the value in memory at the given address
 */
u8 MemoryMapForZxSpectrum48k::read(u16 address)
{
    if (address <= s_address_ram_end) {
        return m_memory.direct_read(address);
    } else {
        return 0;
    }
}
}
