#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"

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
    void add(Data &acc_reg, Address address, const EmulatorMemory<Address, Data> &memory) {
        acc_reg = acc_reg + memory.read(address);
    }
}
