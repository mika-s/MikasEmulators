#include <iostream>
#include "doctest.h"
#include "chips/z80/emulator_memory.h"
#include "chips/z80/flags.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    using emu::util::byte::low_byte;
    using emu::util::byte::high_byte;
    using emu::util::byte::to_u16;

    /**
     * Exchange registers
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex(u8 &acc_reg, Flags &flag_reg, u8 &acc_p_reg, Flags &flag_p_reg, cyc &cycles) {
        const u8 acc_temp = acc_reg;
        acc_reg = acc_p_reg;
        acc_p_reg = acc_temp;

        Flags flags_temp;
        flags_temp = flag_reg;
        flag_reg = flag_p_reg;
        flag_p_reg = flags_temp;

        cycles = 4;
    }

    void ex_msp_dd(u16 sp, EmulatorMemory &memory, u16 &reg) {
        const u16 previous_reg = reg;
        reg = to_u16(memory.read(sp + 1), memory.read(sp));
        memory.write(sp, low_byte(previous_reg));
        memory.write(sp + 1, high_byte(previous_reg));
    }

    /**
     * Exchange memory at SP's location with HL
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 19</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer
     * @param memory is the memory, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex_msp_hl(u16 sp, EmulatorMemory &memory, u8 &h_reg, u8 &l_reg, cyc &cycles) {
        u16 hl = to_u16(h_reg, l_reg);
        ex_msp_dd(sp, memory, hl);
        h_reg = high_byte(hl);
        l_reg = low_byte(hl);

        cycles = 19;
    }

    /**
     * Exchange memory at SP's location with IX or IY
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer
     * @param memory is the memory, which will be mutated
     * @param ixy_reg is the IX or IY register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex_msp_ixy(u16 sp, EmulatorMemory &memory, u16 &ixy_reg, cyc &cycles) {
        ex_msp_dd(sp, memory, ixy_reg);

        cycles = 23;
    }

    /**
     * Exchange H and L with D and E
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param d_reg is the D register, which will be mutated
     * @param e_reg is the E register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ex_de_hl(u8 &h_reg, u8 &l_reg, u8 &d_reg, u8 &e_reg, cyc &cycles) {
        const u8 h = h_reg;
        const u8 l = l_reg;
        h_reg = d_reg;
        l_reg = e_reg;
        d_reg = h;
        e_reg = l;

        cycles = 4;
    }

    void print_ex(std::ostream &ostream, const std::string &reg, const std::string &comp_reg) {
        ostream << "EX "
                << reg
                << ","
                << comp_reg;
    }

    TEST_CASE("Z80: EX") {
        cyc cycles = 0;

        SUBCASE("should exchange AF with AF'") {
            cycles = 0;

            u8 acc_reg = 10;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            u8 acc_p_reg = 4;
            Flags flag_p_reg;
            flag_p_reg.set_half_carry_flag();

            ex(acc_reg, flag_reg, acc_p_reg, flag_p_reg, cycles);

            CHECK_EQ(4, acc_reg);
            CHECK_EQ(10, acc_p_reg);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());

            CHECK_EQ(true, flag_p_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_p_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_p_reg.is_add_subtract_flag_set());
            CHECK_EQ(false, flag_p_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_p_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_p_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            u8 acc_reg = 10;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            u8 acc_p_reg = 4;
            Flags flag_p_reg;
            flag_reg.set_half_carry_flag();

            ex(acc_reg, flag_reg, acc_p_reg, flag_p_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: EX (SP), HL") {
        cyc cycles = 0;

        SUBCASE("should exchange HL with memory at stack pointer") {
            EmulatorMemory memory;
            memory.add({0x33, 0x44});
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 sp = 0x00;

            ex_msp_hl(sp, memory, h_reg, l_reg, cycles);

            CHECK_EQ(0x44, h_reg);
            CHECK_EQ(0x33, l_reg);
            CHECK_EQ(0x22, memory.read(sp));
            CHECK_EQ(0x11, memory.read(sp + 1));
        }

        SUBCASE("should use 18 cycles") {
            cycles = 0;
            EmulatorMemory memory;
            memory.add({0x33, 0x44});
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 sp = 0x00;

            ex_msp_hl(sp, memory, h_reg, l_reg, cycles);

            CHECK_EQ(19, cycles);
        }
    }

    TEST_CASE("Z80: EX (SP), IX") {
        cyc cycles = 0;

        SUBCASE("should exchange IX with memory at stack pointer") {
            EmulatorMemory memory;
            memory.add({0x90, 0x48});
            u16 ix_reg = 0x3988;
            u8 sp = 0x00;

            ex_msp_ixy(sp, memory, ix_reg, cycles);

            CHECK_EQ(0x4890, ix_reg);
            CHECK_EQ(0x88, memory.read(sp));
            CHECK_EQ(0x39, memory.read(sp + 1));
        }

        SUBCASE("should use 23 cycles") {
            cycles = 0;
            EmulatorMemory memory;
            memory.add({0x33, 0x44});
            u16 ix_reg = 0x1122;
            u8 sp = 0x00;

            ex_msp_ixy(sp, memory, ix_reg, cycles);

            CHECK_EQ(23, cycles);
        }
    }

    TEST_CASE("Z80: EX DE, HL") {
        cyc cycles = 0;

        SUBCASE("should exchange HL with DE") {
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 d_reg = 0x33;
            u8 e_reg = 0x44;

            ex_de_hl(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(0x33, h_reg);
            CHECK_EQ(0x44, l_reg);
            CHECK_EQ(0x11, d_reg);
            CHECK_EQ(0x22, e_reg);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u8 d_reg = 0;
            u8 e_reg = 0;

            ex_de_hl(h_reg, l_reg, d_reg, e_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
