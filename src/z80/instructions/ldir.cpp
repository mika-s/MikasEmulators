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
     * Load and increment repeated
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
    void ldir(u16 &pc, u8 &b_reg, u8 &c_reg, u8 &d_reg, u8 &e_reg, u8 &h_reg, u8 &l_reg,
              u8 acc_reg, EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles) {
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

        ++de;
        d_reg = second_byte(de);
        e_reg = first_byte(de);

        ++hl;
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

    void print_ldir(std::ostream &ostream) {
        ostream << "LDIR";
    }

    TEST_CASE("Z80: LDIR") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should transfer from (HL) to (DE) once, when BC is 1") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 1;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(0, pc);
            CHECK_EQ(0x23, memory[to_u16(d_reg, e_reg) - 1]);
            CHECK_EQ(0x23, memory[to_u16(h_reg, l_reg) - 1]);
            CHECK_EQ(0, to_u16(b_reg, c_reg));
            CHECK_EQ(5, to_u16(d_reg, e_reg));
            CHECK_EQ(1, to_u16(h_reg, l_reg));
        }

        SUBCASE("should always reset the half carry flag") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 1;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;
            flag_reg.set_half_carry_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            flag_reg.set_half_carry_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always reset the add/subtract flag") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 1;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            flag_reg.set_add_subtract_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should set the parity/overflow flag while BC is greater than 0, and reset it when it reaches 0") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 3;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 21 cycles when it's not the last repetition") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 2;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(21, cycles);
        }

        SUBCASE("should use 16 cycles when it's the last repetition") {
            u16 pc = 0;
            EmulatorMemory memory;
            memory.add({0x23, 0x56, 0x3a, 0x99, 0x3f, 0xaa});
            u8 b_reg = 0, c_reg = 1;
            u8 d_reg = 0, e_reg = 4;
            u8 h_reg = 0, l_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            ldir(pc, b_reg, c_reg, d_reg, e_reg, h_reg, l_reg,
                 acc_reg, memory, flag_reg, cycles);

            CHECK_EQ(16, cycles);
        }
    }
}
