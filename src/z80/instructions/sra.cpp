#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::is_bit_set;

    void sra(u8 &value, Flags &flag_reg) {
        const bool should_set_carry_flag = is_bit_set(value, lsb);
        value = value >> 1;
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(value);
    }

    /**
     * Shift register right
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to rotate, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sra_r(u8 &reg, Flags &flag_reg, unsigned long &cycles) {
        sra(reg, flag_reg);

        cycles = 8;
    }

    /**
     * Shift the value in memory at HL's address right
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param value_in_hl is the value in memory at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sra_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles) {
        sra(value_in_hl, flag_reg);

        cycles = 15;
    }

    /**
     * Shift value in memory pointed to by IX or IY plus d right
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sra_MixyPd(u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg,
                    unsigned long &cycles
    ) {
        u8 value = memory[ixy_reg + static_cast<i8>(args.farg)];
        sra(value, flag_reg);

        cycles = 23;
    }

    /**
     * Shift value in memory pointed to by IX or IY plus d right
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param reg is the register store the result in, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sra_MixyPd_r(u8 &reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg,
                      unsigned long &cycles
    ) {
        u8 value = memory[ixy_reg + static_cast<i8>(args.farg)];
        sra(value, flag_reg);
        reg = value;

        cycles = 23;
    }

    void print_sra(std::ostream &ostream, const std::string &reg) {
        ostream << "SRA "
                << reg;
    }
}
