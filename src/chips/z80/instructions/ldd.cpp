#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "chips/z80/emulator_memory.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::low_byte;
    using emu::util::byte::high_byte;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::to_u16;

    /**
     * Load and decrement
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: half carry, parity/overflow, add/subtract</li>
     * </ul>
     *
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
    void ldd(u8 &b_reg, u8 &c_reg, u8 &d_reg, u8 &e_reg, u8 &h_reg, u8 &l_reg, u8 acc_reg,
             EmulatorMemory &memory, Flags &flag_reg, cyc &cycles
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
        d_reg = high_byte(de);
        e_reg = low_byte(de);

        --hl;
        h_reg = high_byte(hl);
        l_reg = low_byte(hl);

        u16 bc = to_u16(b_reg, c_reg);
        --bc;
        b_reg = high_byte(bc);
        c_reg = low_byte(bc);

        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();

        if (bc == 0) {
            flag_reg.clear_parity_overflow_flag();
        } else {
            flag_reg.set_parity_overflow_flag();
        }

        cycles = 16;
    }

    void print_ldd(std::ostream &ostream) {
        ostream << "LDD";
    }
}
