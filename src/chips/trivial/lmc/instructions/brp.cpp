#include "crosscutting/typedefs.h"
#include "flags.h"

namespace emu::lmc {

    /**
     * Branch if positive
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param address is the address to the value in memory
     * @param flag_reg is the flag register
     */
    void brp(u8 &pc, u8 address, Flags flag_reg) {
        if (!flag_reg.is_negative_flag_set()) {
            pc = address;
        }
    }
}
