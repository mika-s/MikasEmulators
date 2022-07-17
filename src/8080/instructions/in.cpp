#include <stdexcept>
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
     * Input from port
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the input port number
     * @param io is the IO addresses
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void in(u8 &acc_reg, const NextByte &args, std::vector<u8> io, unsigned long &cycles) {
        acc_reg = io[args.farg];

        cycles = 10;
    }

    void print_in(std::ostream &ostream, const NextByte &args) {
        ostream << "IN "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: IN") {
        unsigned long cycles = 0;
        std::vector<u8> io = {0, 2, 4, 6, 8, 10};
        NextByte args = {.farg = 0x1};
        u8 acc_reg = 0;

        SUBCASE("should store addressed IO in the accumulator") {
            in(acc_reg, args, io, cycles);

            CHECK_EQ(io[args.farg], acc_reg);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            in(acc_reg, args, io, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
