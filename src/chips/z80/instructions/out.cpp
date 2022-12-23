#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::z80 {

using emu::memory::NextByte;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

/**
 * Output to port using immediate value
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 11</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param args contains the argument with the output port number
 * @param io is the IO addresses, which might be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void out_n_A(u8 acc_reg, NextByte const& args, std::vector<u8>& io, cyc& cycles)
{
    io[to_u16(acc_reg, args.farg)] = acc_reg;

    cycles = 11;
}

/**
 * Output to port using the C register
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 12</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param b_reg is the B register
 * @param c_reg is the C register
 * @param reg is the register that will be stored in IO
 * @param io is the IO addresses, which might be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void out_C_r(u8 b_reg, u8 c_reg, u8 reg, std::vector<u8>& io, cyc& cycles)
{
    io[to_u16(b_reg, c_reg)] = reg;

    cycles = 12;
}

void print_out(std::ostream& ostream, NextByte const& args)
{
    ostream << "OUT "
            << hexify_wo_0x(args.farg);
}

void print_out_r_r(std::ostream& ostream, std::string const& dest, std::string const& src)
{
    ostream << "OUT "
            << "("
            << dest
            << "),"
            << src;
}

void print_out_r_r_undocumented(std::ostream& ostream, std::string const& dest, std::string const& src)
{
    ostream << "OUT "
            << "("
            << dest
            << "),"
            << src
            << "*";
}

TEST_CASE("Z80: OUT (n), A")
{
    cyc cycles = 0;
    std::vector<u8> io = { 0, 2, 4, 6, 8, 10 };
    NextByte args = { .farg = 0x1 };
    u8 acc_reg = 100;

    SUBCASE("should store the accumulator in the addressed IO")
    {
        out_n_A(acc_reg, args, io, cycles);

        CHECK_EQ(acc_reg, io[args.farg]);
    }

    SUBCASE("should use 11 cycles")
    {
        cycles = 0;

        out_n_A(acc_reg, args, io, cycles);

        CHECK_EQ(11, cycles);
    }
}
}
