#include <cstdint>
#include <iostream>
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    /**
     * Decrement register pair
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register in the register pair, which will be mutated
     * @param reg2 is the second register in the register pair, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dcx(std::uint8_t &reg1, std::uint8_t &reg2, unsigned long &cycles) {
        std::uint16_t val = emu::util::byte::to_uint16_t(reg1, reg2);
        --val;

        reg2 = emu::util::byte::first_byte(val);
        reg1 = emu::util::byte::second_byte(val);

        cycles = 5;
    }

    /**
     * Decrement register pair (SP)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dcx_sp(std::uint16_t &sp, unsigned long &cycles) {
        --sp;

        cycles = 5;
    }

    void print_dcx(const std::string &reg) {
        std::cout << "DCX " << reg;
    }
}
