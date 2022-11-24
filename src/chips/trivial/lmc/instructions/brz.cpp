#include "chips/trivial/lmc/usings.h"

namespace emu::lmc {

    /**
     * Branch if zero
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param pc is the program counter, which will be mutated
     * @param address is the address to the value in memory
     */
    void brz(Data acc_reg, Address &pc, Address address) {
        if (acc_reg.underlying() == 0) {
            pc = address;
        }
    }
}
