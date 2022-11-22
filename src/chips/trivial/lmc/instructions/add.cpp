#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::lmc {

    using emu::memory::EmulatorMemory;


    /**
     * Add value in memory to the accumulator
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param address is the address to the value in memory
     * @param memory is the memory
     */
    void add(u16 &acc_reg, u8 address, const EmulatorMemory<u8, u16> &memory) {
        acc_reg += memory.read(address);
    }
}
