#include <cassert>
#include <iostream>
#include "doctest.h"
#include "chips/z80/emulator_memory.h"
#include "chips/z80/flags.h"
#include "chips/z80/util.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::set_bit;

    /**
     * Set bit (register)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param bit_number is the bit number to set
     * @param reg is the register to set a bit in, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void set_r(unsigned int bit_number, u8 &reg, cyc &cycles) {
        assert(bit_number < 8);

        set_bit(reg, bit_number);

        cycles = 8;
    }

    /**
     * Set bit (value of memory location pointed to by HL)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param bit_number is the bit number to set
     * @param hl_reg is the HL register pair
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void set_MHL(unsigned int bit_number, u16 hl_reg, EmulatorMemory &memory, cyc &cycles) {
        assert(bit_number < 8);

        set_bit_in_memory(memory, hl_reg, bit_number);

        cycles = 15;
    }

    /**
     * Set bit (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param bit_number is the bit number to set
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d contains address offset
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void set_MixyPd(unsigned int bit_number, u16 ixy_reg, u8 d, EmulatorMemory &memory,
                    cyc &cycles
    ) {
        assert(bit_number < 8);

        set_bit_in_memory(memory, ixy_reg + static_cast<i8>(d), bit_number);

        cycles = 23;
    }

    void print_set(std::ostream &ostream, unsigned int bit_number, const std::string &src) {
        assert(bit_number < 8);

        ostream << "SET "
                << bit_number
                << ", "
                << src;
    }

    void print_set_MixyPn(std::ostream &ostream, unsigned int bit_number, const std::string &ixy_reg, u8 d) {
        assert(bit_number < 8);

        const i8 signed_value = static_cast<i8>(d);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "SET "
                << bit_number
                << ", ("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }
}
