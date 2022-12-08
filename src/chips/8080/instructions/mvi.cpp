#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::i8080 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::util::string::hexify_wo_0x;

void mvi(u8& reg, NextByte const& args)
{
    reg = args.farg;
}

/**
 * Move immediate
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param reg is the register to load into, which will be mutated
 * @param args contains value to load into the register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void mvi_r(u8& reg, NextByte const& args, cyc& cycles)
{
    mvi(reg, args);

    cycles = 7;
}

/**
 * Move immediate value in memory
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param memory is the memory, which will be mutated
 * @param address is the address in memory to move immediate to
 * @param args contains value to load into the register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void mvi_m(EmulatorMemory<u16, u8>& memory, u16 address, NextByte const& args, cyc& cycles)
{
    u8 value = memory.read(address);

    mvi(value, args);

    memory.write(address, value);

    cycles = 10;
}

void print_mvi(std::ostream& ostream, std::string const& reg, NextByte const& args)
{
    ostream << "MVI "
            << reg << ","
            << hexify_wo_0x(args.farg);
}

TEST_CASE("8080: MVI")
{
    cyc cycles = 0;

    SUBCASE("should load registers with value")
    {
        u8 reg = 0;
        NextByte args = { .farg = 0 };

        mvi_r(reg, args, cycles);
        CHECK_EQ(0, reg);

        args = { .farg = 0xa };
        mvi_r(reg, args, cycles);
        CHECK_EQ(0xa, reg);

        mvi_r(reg, args, cycles);
        CHECK_EQ(0xa, reg);

        args = { .farg = 0xff };
        mvi_r(reg, args, cycles);
        CHECK_EQ(0xff, reg);
    }

    SUBCASE("should load memory with value")
    {
        NextByte args = { .farg = 0 };
        EmulatorMemory<u16, u8> memory;
        memory.add({ 0x10, 0x15, 0x01, 0xaa });
        u16 address = 0x0000;

        mvi_m(memory, address, args, cycles);
        CHECK_EQ(0, memory.read(address));

        args = { .farg = 0xa };
        mvi_m(memory, address + 1, args, cycles);
        CHECK_EQ(0xa, memory.read(address + 1));

        mvi_m(memory, address + 2, args, cycles);
        CHECK_EQ(0xa, memory.read(address + 2));

        args = { .farg = 0xff };
        mvi_m(memory, address + 3, args, cycles);
        CHECK_EQ(0xff, memory.read(address + 3));
    }

    SUBCASE("should use 7 cycles if memory is not involved")
    {
        cycles = 0;
        u8 reg = 0;
        NextByte args = { .farg = 0x11 };

        mvi_r(reg, args, cycles);

        CHECK_EQ(7, cycles);
    }

    SUBCASE("should use 10 cycles if memory is involved")
    {
        cycles = 0;
        NextByte args = { .farg = 0x21 };
        EmulatorMemory<u16, u8> memory;
        memory.add({ 0x10 });
        u16 address = 0x0000;

        mvi_m(memory, address, args, cycles);

        CHECK_EQ(10, cycles);
    }
}
}
