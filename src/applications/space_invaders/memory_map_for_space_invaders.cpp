#include "memory_map_for_space_invaders.h"
#include "crosscutting/memory/emulator_memory.h"

namespace emu::applications::space_invaders {

MemoryMapForSpaceInvaders::MemoryMapForSpaceInvaders(EmulatorMemory<u16, u8>& memory)
    : m_memory(memory)
{
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMapForSpaceInvaders::write(u16 address, u8 value)
{
    address &= s_address_mask;

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
u8 MemoryMapForSpaceInvaders::read(u16 address)
{
    address &= s_address_mask;

    if (address <= s_address_ram_end) {
        return m_memory.direct_read(address);
    } else {
        return 0;
    }
}
}
