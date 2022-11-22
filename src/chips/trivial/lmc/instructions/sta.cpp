#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::lmc {

    using emu::memory::EmulatorMemory;

    /**
     * Store value in memory
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param address is the address to the value in memory
     * @param memory is the memory, which will be mutated
     */
    void sta(u16 acc_reg, u8 address, EmulatorMemory<u8, u16> &memory) {
        memory.write(address, acc_reg);
    }
}
