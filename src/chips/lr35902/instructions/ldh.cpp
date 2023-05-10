#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

/**
 * Load from high page into the accumulator
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 12</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator, which will be mutated
 * @param memory is the memory
 * @param args contains the argument with the address
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ldh_A_Mn(u8& acc_reg, EmulatorMemory<u16, u8> const& memory, NextByte const& args, cyc& cycles)
{
    acc_reg = memory.read(to_u16(0xff, args.farg));

    cycles = 12;
}

/**
 * Load from the accumulator into high page
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 12</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator, which will be mutated
 * @param memory is the memory
 * @param args contains the argument with the address
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ldh_Mn_A(EmulatorMemory<u16, u8>& memory, NextByte const& args, u8 acc_reg, cyc& cycles)
{
    memory.write(to_u16(0xff, args.farg), acc_reg);

    cycles = 12;
}

void print_ldh_A_Mn(std::ostream& ostream, NextByte const& args)
{
    ostream << "LDH A"
            << ",("
            << hexify_wo_0x(args.farg)
            << ")";
}

void print_ldh_Mn_A(std::ostream& ostream, NextByte const& args)
{
    ostream << "LDH ("
            << hexify_wo_0x(args.farg)
            << "), A";
}
}
