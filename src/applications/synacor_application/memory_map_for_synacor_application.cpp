#include "memory_map_for_synacor_application.h"
#include "crosscutting/memory/emulator_memory.h"

namespace emu::applications::synacor {

MemoryMapForSynacorApplication::MemoryMapForSynacorApplication(EmulatorMemory<Address, Data>& memory)
    : m_memory(memory)
{
}

/**
 * Called when the CPU writes to memory
 *
 * @param address is the address in memory to write to
 * @param value is the value that should be written to memory
 */
void MemoryMapForSynacorApplication::write(Address address, Data value)
{
    //    if (address <= s_address_rom_end) {
    //    } else if (address <= s_address_ram_end) {
    m_memory.direct_write(address, value);
    //    } else {
    //    }
}

/**
 * Called when the CPU reads from memory
 *
 * @param address is the address in memory to read from
 * @return the value in memory at the given address
 */
Data MemoryMapForSynacorApplication::read(Address address)
{
    //    if (address <= s_address_ram_end) {
    return m_memory.direct_read(address);
    //    } else {
    //        return 0;
    //    }
}
}
