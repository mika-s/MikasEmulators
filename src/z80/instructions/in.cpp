#include <vector>
#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

    /**
     * Input from port
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the input port number
     * @param io is the IO addresses
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void in_A_Mn(u8 &acc_reg, const NextByte &args, std::vector<u8> io, unsigned long &cycles) {
        acc_reg = io[args.farg];

        cycles = 11;
    }

    void print_in(std::ostream &ostream, const NextByte &args) {
        ostream << "IN "
                << hexify_wo_0x(args.farg);
    }

    void print_in_undocumented(std::ostream &ostream, const std::string& dest) {
        ostream << "IN "
                << dest
                << "*";
    }

    void print_in_r_Mr(std::ostream &ostream, const std::string& dest, const std::string& src) {
        ostream << "IN "
                << dest
                << ", ("
                << src
                << ")";
    }

    TEST_CASE("Z80: IN A, [n]") {
        unsigned long cycles = 0;
        std::vector<u8> io = {0, 2, 4, 6, 8, 10};
        NextByte args = {.farg = 0x1};
        u8 acc_reg = 0;

        SUBCASE("should store addressed IO in the accumulator") {
            in_A_Mn(acc_reg, args, io, cycles);

            CHECK_EQ(io[args.farg], acc_reg);
        }

        SUBCASE("should use 11 cycles") {
            cycles = 0;

            in_A_Mn(acc_reg, args, io, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
