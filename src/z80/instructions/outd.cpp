#include <vector>
#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    /**
     * Output and decrement
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: carry, sign, zero, add/subtract</li>
     * </ul>
     *
     * @param b_reg is B register, which will be mutated
     * @param c_reg is C register
     * @param h_reg is H register, which will be mutated
     * @param l_reg is L register, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param io is the IO addresses
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void outd(u8 &b_reg, u8 c_reg, u8 &h_reg, u8 &l_reg, EmulatorMemory &memory, Flags &flag_reg,
              std::vector<u8> io, unsigned long &cycles
    ) {
        u16 hl = to_u16(h_reg, l_reg);

        b_reg--;

        memory[hl] = io[c_reg];

        flag_reg.handle_zero_flag(b_reg);

        hl--;
        h_reg = second_byte(hl);
        l_reg = first_byte(hl);

        flag_reg.set_add_subtract_flag();

        // TODO: The other flags

        cycles = 16;
    }

    void print_outd(std::ostream &ostream) {
        ostream << "OUTD";
    }

    TEST_CASE("Z80: OUTD") {
        unsigned long cycles = 0;
        u8 h_reg = 0;
        u8 l_reg = 0;
        u8 b_reg = 0;
        u8 c_reg = 0;
        std::vector<u8> io = {0, 2, 4, 6, 8, 10};

        SUBCASE("should store IO in HL, decrement HL, decrement B and set correct flags") {
            cycles = 0;
            u16 init_hl = 10;
            h_reg = second_byte(init_hl);
            l_reg = first_byte(init_hl);
            b_reg = 2;
            c_reg = 3;
            Flags flag_reg;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});

            outd(b_reg, c_reg, h_reg, l_reg, memory, flag_reg, io, cycles);

            CHECK_EQ(9, to_u16(h_reg, l_reg));
            CHECK_EQ(1, b_reg);
            CHECK_EQ(6, io[c_reg]);
            CHECK_EQ(6, memory[to_u16(h_reg, l_reg) + 1]);
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            outd(b_reg, c_reg, h_reg, l_reg, memory, flag_reg, io, cycles);

            CHECK_EQ(8, to_u16(h_reg, l_reg));
            CHECK_EQ(0, b_reg);
            CHECK_EQ(6, io[c_reg]);
            CHECK_EQ(6, memory[to_u16(h_reg, l_reg) + 1]);
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;
            Flags flag_reg;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});

            outd(b_reg, c_reg, h_reg, l_reg, memory, flag_reg, io, cycles);

            CHECK_EQ(16, cycles);
        }
    }
}
