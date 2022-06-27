#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Jump if carry
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the address to jump to
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jc(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles) {
        if (flag_reg.is_carry_flag_set()) {
            pc = to_u16(args.sarg, args.farg);
        }

        cycles = 10;
    }

    void print_jc(std::ostream &ostream, const NextWord &args) {
        ostream << "JC "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: JC") {
        unsigned long cycles = 0;

        SUBCASE("should jump when the carry flag is set") {
            u16 pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jc(pc, args, flag_reg, cycles);

            CHECK_EQ(emu::util::byte::to_u16(args.sarg, args.farg), pc);
        }

        SUBCASE("should not jump when the carry flag is unset") {
            u16 pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jc(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            u16 pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jc(pc, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;
            u16 pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jc(pc, args, flag_reg, cycles);

            CHECK_EQ(10, cycles);

            flag_reg.clear_carry_flag();

            jc(pc, args, flag_reg, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
