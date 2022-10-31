#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    using emu::memory::EmulatorMemory;

    /**
     * Rotate left digit
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 18</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rld(u8 &acc_reg, EmulatorMemory &memory, u16 address, Flags &flag_reg, cyc &cycles) {
        u8 value = memory.read(address);
        u8 new_acc = acc_reg;

        new_acc = (new_acc & 0xf0) | ((value & 0xf0) >> 4);
        value = ((value & 0x0f) << 4) | (acc_reg & 0x0f);

        memory.write(address, value);

        acc_reg = new_acc;

        flag_reg.clear_half_carry_flag();
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(acc_reg);

        cycles = 18;
    }

    void print_rld(std::ostream &ostream) {
        ostream << "RLD";
    }
}
