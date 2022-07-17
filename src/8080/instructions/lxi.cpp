#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_word.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextWord;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Load register pair immediate
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to load into, which will be mutated
     * @param reg2 is the second register to load into, which will be mutated
     * @param args contains value to load into the registers
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lxi(u8 &reg1, u8 &reg2, const NextWord &args, unsigned long &cycles) {
        reg1 = args.sarg;
        reg2 = args.farg;

        cycles = 10;
    }

    /**
     * Load register pair immediate (for SP)
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lxi_sp(u16 &sp, const NextWord &args, unsigned long &cycles) {
        sp = to_u16(args.sarg, args.farg);

        cycles = 10;
    }

    void print_lxi(std::ostream &ostream, const std::string &reg, const NextWord &args) {
        ostream << "LXI "
                << reg << ","
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: LXI") {
        unsigned long cycles = 0;
        u16 sp = 0xe;
        u8 reg1 = 0xe;
        u8 reg2 = 0;
        NextWord args = {.farg = 0x12, .sarg = 0x3a};

        SUBCASE("should load immediate into register pair") {
            lxi(reg1, reg2, args, cycles);

            CHECK_EQ(args.sarg, reg1);
            CHECK_EQ(args.farg, reg2);
        }

        SUBCASE("should load immediate into SP") {
            lxi_sp(sp, args, cycles);

            CHECK_EQ(to_u16(args.sarg, args.farg), sp);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            lxi(reg1, reg2, args, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
