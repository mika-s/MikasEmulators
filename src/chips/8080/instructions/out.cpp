#include <vector>
#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

    /**
     * Output to port
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the output port number
     * @param io is the IO addresses, which might be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void out(u8 acc_reg, const NextByte &args, std::vector<u8> &io, cyc &cycles) {
        io[args.farg] = acc_reg;

        cycles = 10;
    }

    void print_out(std::ostream &ostream, const NextByte &args) {
        ostream << "OUT "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: OUT") {
        cyc cycles = 0;
        std::vector<u8> io = {0, 2, 4, 6, 8, 10};
        NextByte args = {.farg = 0x1};
        u8 acc_reg = 100;

        SUBCASE("should store the accumulator in the addressed IO") {
            out(acc_reg, args, io, cycles);

            CHECK_EQ(acc_reg, io[args.farg]);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            out(acc_reg, args, io, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
