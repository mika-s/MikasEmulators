#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include <iostream>

namespace emu::lmc {

    using emu::memory::EmulatorMemory;

    /**
     * Load the accumulator with value in memory
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param address is the address to the value in memory
     * @param memory is the memory
     */
    void lda(Data &acc_reg, Address address, const EmulatorMemory<Address, Data> &memory) {
        acc_reg = memory.read(address);
    }

    void print_lda(std::ostream &ostream, Address address) {
        ostream << "LDA "
                << address;
    }
}
