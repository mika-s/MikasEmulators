#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::i8080 {

using emu::memory::NextWord;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

/**
 * Call if positive
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 11 or 17</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory, which will be mutated
 * @param args contains the argument with the address to call
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void cp(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    cycles = 0;

    if (!flag_reg.is_sign_flag_set()) {
        execute_call(pc, sp, memory, args);
        cycles += 6;
    }

    cycles += 11;
}

void print_cp(std::ostream& ostream, NextWord const& args)
{
    ostream << "CP "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

TEST_CASE("8080: CP")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the sign flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_sign_flag();

        cp(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the sign flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_sign_flag();

        cp(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 11 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_sign_flag();

        cp(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(11, cycles);
    }

    SUBCASE("should use 17 cycles when called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_sign_flag();

        cp(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}
}
