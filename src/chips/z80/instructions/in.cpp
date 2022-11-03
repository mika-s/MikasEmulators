#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "flags.h"
#include <iostream>
#include <vector>

namespace emu::z80 {

    using emu::memory::NextByte;
    using emu::util::string::hexify_wo_0x;

    /**
     * Input from port using immediate value
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the input port number
     * @param io is the IO addresses
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void in_A_n(u8 &acc_reg, const NextByte &args, std::vector<u8> io, cyc &cycles) {
        acc_reg = io[args.farg];

        cycles = 11;
    }

    /**
     * Input from port using the C register
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 12</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg is a register, which will be mutated
     * @param c_reg is the C register
     * @param io is the IO addresses
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void in_r_C(u8 &reg, u8 c_reg, std::vector<u8> io, Flags &flag_reg, cyc &cycles) {
        reg = io[c_reg];

        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_parity_flag(reg);
        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();

        cycles = 12;
    }

    void print_in(std::ostream &ostream, const NextByte &args) {
        ostream << "IN "
                << hexify_wo_0x(args.farg);
    }

    void print_in_undocumented(std::ostream &ostream, const std::string &dest) {
        ostream << "IN "
                << dest
                << "*";
    }

    void print_in_r_r(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "IN "
                << dest
                << ", ("
                << src
                << ")";
    }

    TEST_CASE("Z80: IN A, [n]") {
        cyc cycles = 0;
        std::vector<u8> io = {0, 2, 4, 6, 8, 10};
        NextByte args = {.farg = 0x1};
        u8 acc_reg = 0;

        SUBCASE("should store addressed IO in the accumulator") {
            in_A_n(acc_reg, args, io, cycles);

            CHECK_EQ(io[args.farg], acc_reg);
        }

        SUBCASE("should use 11 cycles") {
            cycles = 0;

            in_A_n(acc_reg, args, io, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
