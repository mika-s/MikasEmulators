#include "chips/z80/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::z80 {

    using emu::memory::EmulatorMemory;
    using emu::util::byte::high_byte;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::low_byte;
    using emu::util::byte::to_u16;

    /**
     * Compare repeated (decrement HL)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4 or 5</li>
     *   <li>States: 16 or 21</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow add/subtract</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param b_reg is the B register, which will be mutated
     * @param c_reg is the C register, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param acc_reg is the accumulator
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cpdr(u16 &pc, u8 &b_reg, u8 &c_reg, u8 &h_reg, u8 &l_reg, u8 acc_reg, const EmulatorMemory<u16, u8> &memory,
              Flags &flag_reg, cyc &cycles) {

        const u8 value = memory.read(to_u16(h_reg, l_reg));
        const u8 result = acc_reg - value;

        flag_reg.handle_zero_flag(result);
        flag_reg.handle_sign_flag(result);
        flag_reg.handle_parity_flag(result);
        flag_reg.handle_half_borrow_flag(acc_reg, value, false);
        flag_reg.set_add_subtract_flag();

        if (is_bit_set(acc_reg - value - (flag_reg.is_half_carry_flag_set() ? 1 : 0), 1)) {
            flag_reg.set_y_flag();
        } else {
            flag_reg.clear_y_flag();
        }

        if (is_bit_set(acc_reg - value - (flag_reg.is_half_carry_flag_set() ? 1 : 0), 3)) {
            flag_reg.set_x_flag();
        } else {
            flag_reg.clear_x_flag();
        }

        u16 hl = to_u16(h_reg, l_reg);
        hl--;
        h_reg = high_byte(hl);
        l_reg = low_byte(hl);

        u16 bc = to_u16(b_reg, c_reg);
        bc--;
        b_reg = high_byte(bc);
        c_reg = low_byte(bc);

        if (bc == 0) {
            flag_reg.clear_parity_overflow_flag();
        } else {
            flag_reg.set_parity_overflow_flag();
        }

        if (bc == 0 || result == 0) {
            cycles = 16;
        } else {
            cycles = 21;
            pc -= 2;
        }
    }

    void print_cpdr(std::ostream &ostream) {
        ostream << "CPDR";
    }
}
