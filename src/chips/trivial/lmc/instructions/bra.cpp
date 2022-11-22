#include "crosscutting/typedefs.h"

namespace emu::lmc {

    /**
     * Branch
     * <ul>
     *   <li>Size: 2</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param address is the address to the value in memory
     */
    void bra(u8 &pc, u8 address) {
        pc = address;
    }
}
