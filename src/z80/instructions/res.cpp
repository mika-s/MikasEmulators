#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::set_bit;

    /**
     * Reset the bit of register
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, x, y, add/subtract</li>
     * </ul>
     *
     * @param bit_number is the accumulator register, which will be mutated
     * @param reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void res_r(unsigned int bit_number, u8 reg, unsigned long &cycles) {
        set_bit(reg, bit_number);

        cycles = 8;
    }

    /**
     * Reset the bit of memory location pointed to by HL
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, x, y, add/subtract</li>
     * </ul>
     *
     * @param bit_number is the accumulator register, which will be mutated
     * @param hl_reg is the HL register pair, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void res_MHL(unsigned int bit_number, u16 hl_reg, EmulatorMemory &memory, unsigned long &cycles) {
        set_bit(memory[hl_reg], bit_number);

        cycles = 15;
    }

    void print_res(std::ostream &ostream, unsigned int bit_number, const std::string &src) {
        ostream << "RES "
                << bit_number
                << ", "
                << src;
    }
}
