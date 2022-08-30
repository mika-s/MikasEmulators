#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::is_bit_set;

    void bit(unsigned int bit_number, u8 reg, Flags &flag_reg) {
        const bool is_set = is_bit_set(reg, bit_number);

        if (bit_number == msb && is_set) {
            flag_reg.set_sign_flag();
        } else {
            flag_reg.clear_sign_flag();
        }

        if (is_set) {
            flag_reg.clear_zero_flag();
            flag_reg.clear_parity_overflow_flag();
        } else {
            flag_reg.set_zero_flag();
            flag_reg.set_parity_overflow_flag();
        }

        flag_reg.set_half_carry_flag();
        flag_reg.clear_add_subtract_flag();
    }

    /**
     * Test bit of register
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, x, y, add/subtract</li>
     * </ul>
     *
     * @param bit_number is the bit number to test
     * @param reg is the register to test a bit in
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void bit_r(unsigned int bit_number, u8 reg, Flags &flag_reg, unsigned long &cycles) {
        bit(bit_number, reg, flag_reg);

        const bool is_set = is_bit_set(reg, bit_number);

        if (bit_number == 5 && is_set) {
            flag_reg.set_y_flag();
        } else {
            flag_reg.clear_y_flag();
        }

        if (bit_number == 3 && is_set) {
            flag_reg.set_x_flag();
        } else {
            flag_reg.clear_x_flag();
        }

        cycles = 8;
    }

    /**
     * Test bit of memory location pointed to by HL
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 12</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, x, y, add/subtract</li>
     * </ul>
     *
     * @param bit_number is the bit number to test
     * @param hl_reg is the HL register pair
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void bit_MHL(unsigned int bit_number, u16 hl_reg, const EmulatorMemory &memory, Flags &flag_reg,
                 unsigned long &cycles
    ) {
        bit(bit_number, memory[hl_reg], flag_reg);

        // TODO: Handle X and Y flags

        cycles = 12;
    }

    void print_bit(std::ostream &ostream, unsigned int bit_number, const std::string &src) {
        ostream << "BIT "
                << bit_number
                << ", "
                << src;
    }
}
