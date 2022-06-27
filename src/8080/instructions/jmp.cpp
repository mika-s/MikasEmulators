#include <iostream>
#include "doctest.h"
#include "8080/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Jump
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the address to jump to
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jmp(u16 &pc, const NextWord &args, unsigned long &cycles) {
        pc = to_u16(args.sarg, args.farg);

        cycles = 10;
    }

    void print_jmp(std::ostream &ostream, const NextWord &args) {
        ostream << "JMP "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: JMP") {
        unsigned long cycles = 0;
        u16 pc = 0;
        NextWord args = {.farg = 0x11, .sarg = 0x22};

        SUBCASE("should jump") {
            jmp(pc, args, cycles);

            CHECK_EQ(emu::util::byte::to_u16(args.sarg, args.farg), pc);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            jmp(pc, args, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
