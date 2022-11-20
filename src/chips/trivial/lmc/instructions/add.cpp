#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::lmc {

    using emu::memory::EmulatorMemory;


    /**
     * Add from memory to accumulator
     * <ul>
     *   <li>Size: 1</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param address is the value to add to the accumulator register
     * @param memory is the memory
     */
    void add(u8 &acc_reg, u8 address, const EmulatorMemory &memory) {
        acc_reg += memory.read(address);
    }
}
