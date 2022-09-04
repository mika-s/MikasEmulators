#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/emulator_memory.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::to_u16;

    /**
     * Load and decrement repeated
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4 or 5</li>
     *   <li>States: 16 or 21</li>
     *   <li>Condition bits affected: half carry, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param b_reg is the B register, which will be mutated
     * @param c_reg is the C register, which will be mutated
     * @param d_reg is the D register, which will be mutated
     * @param e_reg is the E register, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param acc_reg is the accumulator, for use in setting XF and YF
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lddr(u16 &pc, u8 &b_reg, u8 &c_reg, u8 &d_reg, u8 &e_reg, u8 &h_reg, u8 &l_reg,
              u8 acc_reg, EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles
    ) {
        u16 de = to_u16(d_reg, e_reg);
        u16 hl = to_u16(h_reg, l_reg);
        memory[de] = memory[hl];

        if (is_bit_set(acc_reg + memory[hl], 1)) {
            flag_reg.set_y_flag();
        } else {
            flag_reg.clear_y_flag();
        }

        if (is_bit_set(acc_reg + memory[hl], 3)) {
            flag_reg.set_x_flag();
        } else {
            flag_reg.clear_x_flag();
        }

        --de;
        d_reg = second_byte(de);
        e_reg = first_byte(de);

        --hl;
        h_reg = second_byte(hl);
        l_reg = first_byte(hl);

        u16 bc = to_u16(b_reg, c_reg);
        --bc;
        b_reg = second_byte(bc);
        c_reg = first_byte(bc);

        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();

        if (bc == 0) {
            flag_reg.clear_parity_overflow_flag();
            cycles = 16;
        } else {
            flag_reg.set_parity_overflow_flag();
            cycles = 21;
            pc -= 2;
        }
    }

    void print_lddr(std::ostream &ostream) {
        ostream << "LDDR";
    }
}
