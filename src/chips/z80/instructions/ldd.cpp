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
void ldd(u8& b_reg, u8& c_reg, u8& d_reg, u8& e_reg, u8& h_reg, u8& l_reg, u8 acc_reg,
    EmulatorMemory<u16, u8>& memory, Flags& flag_reg, cyc& cycles)
{
    u16 de = to_u16(d_reg, e_reg);
    u16 hl = to_u16(h_reg, l_reg);
    memory.write(de, memory.read(hl));

    if (is_bit_set(acc_reg + memory.read(hl), 1)) {
        flag_reg.set_y_flag();
    } else {
        flag_reg.clear_y_flag();
    }

    if (is_bit_set(acc_reg + memory.read(hl), 3)) {
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

void print_ldd(std::ostream& ostream)
{
    ostream << "LDD";
}

TEST_CASE("Z80: LDD")
{
    SUBCASE("should load memory at DE with value in HL and then decrement")
    {
        u8 b_reg = 0;
        u8 c_reg = 100;
        u8 d_reg = 0;
        u8 e_reg = 2;
        u8 h_reg = 0;
        u8 l_reg = 6;
        u8 acc_reg = 0;
        EmulatorMemory<u16, u8> memory;
        memory.add({ 0x01, 0x42, 0x5a, 0xbb, 0xc3, 0x10, 0x00 });
        Flags flag_reg;
        cyc cycles;

        ldd(b_reg, c_reg, d_reg, e_reg, h_reg, l_reg, acc_reg, memory, flag_reg, cycles);

        CHECK_EQ(99, to_u16(b_reg, c_reg));
        CHECK_EQ(1, to_u16(d_reg, e_reg));
        CHECK_EQ(5, to_u16(h_reg, l_reg));
        CHECK_EQ(0x00, memory.read(2));
        CHECK_EQ(0x00, memory.read(6));
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        CHECK_EQ(to_u16(b_reg, c_reg) != 0, flag_reg.is_parity_overflow_flag_set());

        ldd(b_reg, c_reg, d_reg, e_reg, h_reg, l_reg, acc_reg, memory, flag_reg, cycles);

        CHECK_EQ(98, to_u16(b_reg, c_reg));
        CHECK_EQ(0, to_u16(d_reg, e_reg));
        CHECK_EQ(4, to_u16(h_reg, l_reg));
        CHECK_EQ(0x10, memory.read(1));
        CHECK_EQ(0x10, memory.read(5));
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        CHECK_EQ(to_u16(b_reg, c_reg) != 0, flag_reg.is_parity_overflow_flag_set());

        ldd(b_reg, c_reg, d_reg, e_reg, h_reg, l_reg, acc_reg, memory, flag_reg, cycles);

        CHECK_EQ(97, to_u16(b_reg, c_reg));
        CHECK_EQ(0xffff, to_u16(d_reg, e_reg));
        CHECK_EQ(3, to_u16(h_reg, l_reg));
        CHECK_EQ(0xc3, memory.read(0));
        CHECK_EQ(0xc3, memory.read(4));
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        CHECK_EQ(to_u16(b_reg, c_reg) != 0, flag_reg.is_parity_overflow_flag_set());
    }
}
}
