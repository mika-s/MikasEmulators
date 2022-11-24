#include "chips/trivial/lmc/usings.h"

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
    void bra(Address &pc, Address address) {
        pc = address;
    }
}
