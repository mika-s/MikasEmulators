#include "crosscutting/typedefs.h"

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
    void brz(u16 acc_reg, u8 &pc, u8 address) {
        if (acc_reg == 0) {
            pc = address;
        }
    }
}
