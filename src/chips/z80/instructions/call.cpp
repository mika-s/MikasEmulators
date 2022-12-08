#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "flags.h"
#include "instruction_util.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::z80 {

using emu::memory::NextWord;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

/**
 * Call
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 5</li>
 *   <li>States: 17</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory, which will be mutated
 * @param args contains the argument with the address to call
 * @param cycles is the number of cycles variable, which will be mutated
 */
void call(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, cyc& cycles)
{
    execute_call(pc, sp, memory, args);

    cycles = 17;
}

/**
 * Call if not zero
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_nz(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (!flag_reg.is_zero_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if zero
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_z(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (flag_reg.is_zero_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if no carry
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_nc(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (!flag_reg.is_carry_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if carry
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_c(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (flag_reg.is_carry_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if parity odd
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_po(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (!flag_reg.is_parity_overflow_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if parity even
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_pe(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (flag_reg.is_parity_overflow_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if positive
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_p(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (!flag_reg.is_sign_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

/**
 * Call if minus
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3 or 5</li>
 *   <li>States: 10 or 17</li>
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
void call_m(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, Flags const& flag_reg,
    cyc& cycles)
{
    if (flag_reg.is_sign_flag_set()) {
        call(pc, sp, memory, args, cycles);
    } else {
        cycles = 10;
    }
}

void print_call_nn(std::ostream& ostream, NextWord const& args)
{
    ostream << "CALL "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

void print_call_cc_nn(std::ostream& ostream, NextWord const& args, std::string const& condition)
{
    ostream << "CALL "
            << condition
            << ", "
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

TEST_CASE("Z80: CALL")
{
    cyc cycles = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA });
    NextWord args = { .farg = 0x2, .sarg = 0x0 };

    SUBCASE("should push current PC on the stack and change PC to the address in args")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;

        call(pc, sp, memory, args, cycles);

        CHECK_EQ(0x0002, pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should use 17 cycles")
    {
        cycles = 0;

        u16 pc = 0;
        u16 sp = 0x2;

        call(pc, sp, memory, args, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL NZ")
{
    cyc cycles = 0;

    SUBCASE("should push the current PC on the stack and change PC to the address in args when zero flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        call_nz(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the zero flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        call_nz(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;

        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        call_nz(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.clear_zero_flag();

        call_nz(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL Z")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when zero flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_zero_flag();

        call_z(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the zero flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        call_z(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        call_z(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.set_zero_flag();

        call_z(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL NC")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the carry flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        call_nc(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the carry flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        call_nc(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        call_nc(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.clear_carry_flag();

        call_nc(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL C")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the carry flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_carry_flag();

        call_c(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when carry flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        call_c(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_carry_flag();

        call_c(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.set_carry_flag();

        call_c(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL PO")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the parity flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_parity_overflow_flag();

        call_po(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the parity flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_parity_overflow_flag();

        call_po(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_parity_overflow_flag();

        call_po(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.clear_parity_overflow_flag();

        call_po(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL PE")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the parity flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_parity_overflow_flag();

        call_pe(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the parity flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_parity_overflow_flag();

        call_pe(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_parity_overflow_flag();

        call_pe(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.set_parity_overflow_flag();

        call_pe(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL P")
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

        call_p(pc, sp, memory, args, flag_reg, cycles);

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

        call_p(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_sign_flag();

        call_p(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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

        call_p(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}

TEST_CASE("Z80: CALL M")
{
    cyc cycles = 0;

    SUBCASE("should push current PC on the stack and change PC to the address in args when the sign flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.set_sign_flag();

        call_m(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), pc);
        CHECK_EQ(0x0f, memory.read(0));
        CHECK_EQ(0x10, memory.read(1));
    }

    SUBCASE("should not do anything when the sign flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_sign_flag();

        call_m(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0x00, memory.read(0));
        CHECK_EQ(0x01, memory.read(1));
    }

    SUBCASE("should use 10 cycles when not called")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0x2;
        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
        NextWord args = { .farg = 0x2, .sarg = 0x0 };
        Flags flag_reg;
        flag_reg.clear_sign_flag();

        call_m(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(10, cycles);
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
        flag_reg.set_sign_flag();

        call_m(pc, sp, memory, args, flag_reg, cycles);

        CHECK_EQ(17, cycles);
    }
}
}
