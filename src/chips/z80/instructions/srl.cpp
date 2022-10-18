#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;

    void srl(u8 &value, Flags &flag_reg) {
        const bool should_set_carry_flag = is_bit_set(value, lsb);
        value = value >> 1;
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        flag_reg.clear_half_carry_flag();
        flag_reg.handle_zero_flag(value);
        flag_reg.handle_sign_flag(value);
        flag_reg.handle_parity_flag(value);
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(value);
    }

    /**
     * Shift right logical (register)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to rotate, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void srl_r(u8 &reg, Flags &flag_reg, cyc &cycles) {
        srl(reg, flag_reg);

        cycles = 8;
    }

    /**
     * Shift right logical (value in memory at HL's address)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param value_in_hl is the value in memory at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void srl_MHL(u8 &value_in_hl, Flags &flag_reg, cyc &cycles) {
        srl(value_in_hl, flag_reg);

        cycles = 15;
    }

    /**
     * Shift right logical (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d is the address offset
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void srl_MixyPd(u16 ixy_reg, u8 d, EmulatorMemory &memory, Flags &flag_reg, cyc &cycles) {
        u8 &value = memory[ixy_reg + static_cast<i8>(d)];
        srl(value, flag_reg);

        cycles = 23;
    }

    /**
     * Shift right logical (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param reg is the register store the result in, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d is the address offset
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void srl_MixyPd_r(u8 &reg, u16 ixy_reg, u8 d, EmulatorMemory &memory, Flags &flag_reg, cyc &cycles) {
        u8 &value = memory[ixy_reg + static_cast<i8>(d)];
        srl(value, flag_reg);
        reg = value;

        cycles = 23;
    }

    void print_srl(std::ostream &ostream, const std::string &reg) {
        ostream << "SRL "
                << reg;
    }

    void print_srl_MixyPn(std::ostream &ostream, unsigned int bit_number, const std::string &ixy_reg, u8 d) {
        const i8 signed_value = static_cast<i8>(d);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "SRL "
                << bit_number
                << ", ("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }
}
