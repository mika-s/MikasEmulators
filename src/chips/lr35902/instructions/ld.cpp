#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::string::hexify_wo_0x;

void ld(u8& to, u8 value)
{
    to = value;
}

void ld_M(EmulatorMemory<u16, u8>& memory, u16 address, u8 value)
{
    memory.write(address, value);
}

/**
 * Load from register to register
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the register or memory location to move value to
 * @param value is the value to move into to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_r_r(u8& to, u8 value, cyc& cycles)
{
    ld(to, value);

    cycles = 4;
}

/**
 * Load immediate into register
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the register to load into, which will be mutated
 * @param args contains value to load into the register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_r_n(u8& to, NextByte const& args, cyc& cycles)
{
    ld(to, args.farg);

    cycles = 7;
}

/**
 * Load value in memory at HL's address into register
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the register to load into, which will be mutated
 * @param value is the value to load into to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_r_MHL(u8& to, u8 value, cyc& cycles)
{
    ld(to, value);

    cycles = 7;
}

/**
 * Load register into memory at HL's address
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the memory location to load into, which will be mutated
 * @param value is the register value to load into to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_MHL_r(EmulatorMemory<u16, u8>& memory, u16 address, u8 value, cyc& cycles)
{
    ld_M(memory, address, value);

    cycles = 7;
}

/**
 * Load immediate value into memory in HL's address
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param reg is the register to load into, which will be mutated
 * @param address is the address in memory to load from
 * @param args contains value to load into the register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_MHL_n(EmulatorMemory<u16, u8>& memory, u16 address, NextByte const& args, cyc& cycles)
{
    ld_M(memory, address, args.farg);

    cycles = 10;
}

/**
 * Load from memory into register
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the register or memory location to move value to
 * @param value is the value to move into to_reg
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_A_Mss(u8& to, u8 value, cyc& cycles)
{
    ld(to, value);

    cycles = 7;
}

/**
 * Load accumulator with value in memory
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 13</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator, which will be mutated
 * @param memory is the memory
 * @param args contains the argument with the address
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_A_Mnn(u8& acc_reg, EmulatorMemory<u16, u8> const& memory, NextWord const& args, cyc& cycles)
{
    acc_reg = memory.read(to_u16(args.sarg, args.farg));

    cycles = 13;
}

/**
 * Load the accumulator into memory at HL's address
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param to is the memory location to load into, which will be mutated
 * @param acc_reg is the accumulator value to load into to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_Mss_A(EmulatorMemory<u16, u8>& memory, u16 address, u8 acc_reg, cyc& cycles)
{
    ld_M(memory, address, acc_reg);

    cycles = 7;
}

/**
 * Load the accumulator memory at address nn
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 13</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register
 * @param value is the memory, which will be mutated
 * @param args contains the argument with the address in memory to store the accumulator register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_Mnn_A(u8& acc_reg, EmulatorMemory<u16, u8>& memory, NextWord const& args, cyc& cycles)
{
    const u16 address = to_u16(args.sarg, args.farg);

    memory.write(address, acc_reg);

    cycles = 13;
}

/**
 * Load HL with value in memory
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 5</li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param h_reg is the H register, which will be mutated
 * @param l_reg is the L register, which will be mutated
 * @param memory is the memory
 * @param args contains the argument with the address to the wanted value in memory
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_HL_Mnn(u8& h_reg, u8& l_reg, EmulatorMemory<u16, u8> const& memory, NextWord const& args, cyc& cycles)
{
    const u16 address = to_u16(args.sarg, args.farg);
    h_reg = memory.read(address + 1);
    l_reg = memory.read(address);

    cycles = 16;
}

/**
 * Load register pair with immediate value
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param reg1 is the first register to load into, which will be mutated
 * @param reg2 is the second register to load into, which will be mutated
 * @param args contains value to load into the registers
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_dd_nn(u8& reg1, u8& reg2, NextWord const& args, cyc& cycles)
{
    reg1 = args.sarg;
    reg2 = args.farg;

    cycles = 10;
}

/**
 * Load register pair with the value at a memory location
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 20</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param hi_reg is the high-order portion of the register pair to load into, which will be mutated
 * @param lo_reg is the low-order portion of the register pair to load into, which will be mutated
 * @param args contains the address to load from
 * @param memory is the memory, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_dd_Mnn(u8& hi_reg, u8& lo_reg, NextWord const& args, EmulatorMemory<u16, u8> const& memory, cyc& cycles)
{
    const u16 address = to_u16(args.sarg, args.farg);
    hi_reg = memory.read(address + 1);
    lo_reg = memory.read(address);

    cycles = 20;
}

/**
 * Load register pair immediate (for SP)
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 10</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer, which will be mutated
 * @param args contains value to load into the register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_sp_nn(u16& sp, NextWord const& args, cyc& cycles)
{
    sp = to_u16(args.sarg, args.farg);

    cycles = 10;
}

/**
 * Load content of the HL register pair into SP
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 6</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer, which will be mutated
 * @param hl is the value to store in SP
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_sp_hl(u16& sp, u16 hl, cyc& cycles)
{
    sp = hl;

    cycles = 6;
}

/**
 * Load HL into memory
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 5</li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param h_reg is the H register
 * @param l_reg is the L register
 * @param memory is the memory, which will be mutated
 * @param args contains the argument with the address to call
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_Mnn_HL(u8 h_reg, u8 l_reg, EmulatorMemory<u16, u8>& memory, NextWord const& args, cyc& cycles)
{
    const u16 l_address = to_u16(args.sarg, args.farg);
    const u16 h_address = l_address + 1;

    memory.write(h_address, h_reg);
    memory.write(l_address, l_reg);

    cycles = 16;
}

/**
 * Load register pair into memory
 * <ul>
 *   <li>Size: 3</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 20</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param hi_reg is the high-order portion of the register pair
 * @param lo_reg is the low-order portion of the register pair
 * @param memory is the memory, which will be mutated
 * @param args contains the argument with the address to call
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_Mnn_dd(u8 hi_reg, u8 lo_reg, EmulatorMemory<u16, u8>& memory, NextWord const& args, cyc& cycles)
{
    const u16 lo_address = to_u16(args.sarg, args.farg);
    const u16 hi_address = lo_address + 1;

    memory.write(hi_address, hi_reg);
    memory.write(lo_address, lo_reg);

    cycles = 20;
}

/**
 * Load SP into memory
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 20</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer to store
 * @param memory is the memory, which will be mutated
 * @param args contains the argument with the address to lookup
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_Mnn_sp(u16 sp, EmulatorMemory<u16, u8>& memory, NextWord const& args, cyc& cycles)
{
    ld_Mnn_dd(high_byte(sp), low_byte(sp), memory, args, cycles);
}

/**
 * Load SP with value in memory
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 20</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory
 * @param args contains the argument with the address to lookup
 * @param cycles is the number of cycles variable, which will be mutated
 */
void ld_sp_Mnn(u16& sp, EmulatorMemory<u16, u8> const& memory, NextWord const& args, cyc& cycles)
{
    const u16 first_address = to_u16(args.sarg, args.farg);
    const u16 second_address = first_address + 1;

    const u8 lo = memory.read(first_address);
    const u8 hi = memory.read(second_address);

    sp = to_u16(hi, lo);

    cycles = 20;
}

void print_ld(std::ostream& ostream, std::string const& dest, std::string const& src)
{
    ostream << "LD "
            << dest
            << ","
            << src;
}

void print_ld(std::ostream& ostream, std::string const& dest, NextByte const& args)
{
    ostream << "LD "
            << dest
            << ","
            << hexify_wo_0x(args.farg);
}

void print_ld(std::ostream& ostream, std::string const& dest, NextWord const& args)
{
    ostream << "LD "
            << dest
            << ",("
            << hexify_wo_0x(to_u16(args.sarg, args.farg))
            << ")";
}

void print_ld(std::ostream& ostream, NextWord const& args, std::string const& src)
{
    ostream << "LD ("
            << hexify_wo_0x(to_u16(args.sarg, args.farg))
            << "),"
            << src;
}

void print_ld_dd_nn(std::ostream& ostream, std::string const& dest, NextWord const& args)
{
    ostream << "LD "
            << dest
            << ","
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg);
}

void print_ld_Mnn_dd(std::ostream& ostream, NextWord const& args, std::string const& src)
{
    ostream << "LD ("
            << hexify_wo_0x(args.sarg)
            << hexify_wo_0x(args.farg)
            << "),"
            << src;
}

TEST_CASE("LR35902: LD")
{
    SUBCASE("should load register/memory with value from register/memory")
    {
        u8 reg1 = 0;
        u8 reg2 = 0;

        ld(reg1, reg2);
        CHECK_EQ(0, reg1);

        reg2 = 0xa;
        ld(reg1, reg2);
        CHECK_EQ(0xa, reg1);

        reg2 = 0xff;
        ld(reg1, reg2);
        CHECK_EQ(0xff, reg1);
    }
}

TEST_CASE("LR35902: LD r, r'")
{
    SUBCASE("should use 4 cycles")
    {
        cyc cycles = 0;
        u8 reg1 = 1;
        u8 reg2 = 2;

        ld_r_r(reg1, reg2, cycles);

        CHECK_EQ(4, cycles);
    }
}

TEST_CASE("LR35902: LD r, n")
{
    SUBCASE("should use 7 cycles")
    {
        cyc cycles = 0;
        u8 reg = 0;
        NextByte args = { .farg = 0x11 };

        ld_r_n(reg, args, cycles);

        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: LD (HL), r")
{
    cyc cycles = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    u8 acc_reg = 0;
    u8 reg1 = 0x0;
    u8 reg2 = 0x3;

    SUBCASE("should store the accumulator in memory at the given address")
    {
        ld_MHL_r(memory, to_u16(reg1, reg2), acc_reg, cycles);

        CHECK_EQ(acc_reg, memory.read(0x3));
    }

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;

        ld_MHL_r(memory, to_u16(reg1, reg2), acc_reg, cycles);

        CHECK_EQ(7, cycles);
    }
}

TEST_CASE("LR35902: LD A, (nn)")
{
    cyc cycles = 0;
    u8 reg = 0xe;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    NextWord args = { .farg = 0x04, .sarg = 0 };

    SUBCASE("should load the register from memory using the address in args")
    {
        ld_A_Mnn(reg, memory, args, cycles);

        CHECK_EQ(memory.read(0x04), reg);
    }

    SUBCASE("should use 13 cycles")
    {
        cycles = 0;

        ld_A_Mnn(reg, memory, args, cycles);

        CHECK_EQ(13, cycles);
    }
}

TEST_CASE("LR35902: LD HL, (nn)")
{
    cyc cycles = 0;
    u8 l_reg = 0xe;
    u8 h_reg = 0x42;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    NextWord args = { .farg = 0x04, .sarg = 0 };

    SUBCASE("should load HL from memory using the address in args")
    {
        ld_HL_Mnn(h_reg, l_reg, memory, args, cycles);

        CHECK_EQ(memory.read(0x04), l_reg);
        CHECK_EQ(memory.read(0x05), h_reg);
    }

    SUBCASE("should use 16 cycles")
    {
        cycles = 0;

        ld_HL_Mnn(h_reg, l_reg, memory, args, cycles);

        CHECK_EQ(16, cycles);
    }
}

TEST_CASE("LR35902: LD dd, nn")
{
    cyc cycles = 0;
    u16 sp = 0xe;
    u8 reg1 = 0xe;
    u8 reg2 = 0;
    NextWord args = { .farg = 0x12, .sarg = 0x3a };

    SUBCASE("should load immediate into register pair")
    {
        ld_dd_nn(reg1, reg2, args, cycles);

        CHECK_EQ(args.sarg, reg1);
        CHECK_EQ(args.farg, reg2);
    }

    SUBCASE("should load immediate into SP")
    {
        ld_sp_nn(sp, args, cycles);

        CHECK_EQ(to_u16(args.sarg, args.farg), sp);
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;

        ld_dd_nn(reg1, reg2, args, cycles);

        CHECK_EQ(10, cycles);
    }
}

TEST_CASE("LR35902: LD (nn), HL")
{
    cyc cycles = 0;
    u8 l_reg = 0x22;
    u8 h_reg = 0x11;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0xff, 0xaa, 0xbb, 0xcc, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    NextWord args = { .farg = 0x2, .sarg = 0x0 };

    SUBCASE("should load memory with H and L registers")
    {
        ld_Mnn_HL(h_reg, l_reg, memory, args, cycles);

        CHECK_EQ(h_reg, memory.read(0x03));
        CHECK_EQ(l_reg, memory.read(0x02));
    }

    SUBCASE("should use 16 cycles")
    {
        cycles = 0;

        ld_Mnn_HL(l_reg, h_reg, memory, args, cycles);

        CHECK_EQ(16, cycles);
    }
}

TEST_CASE("LR35902: LD (nn), A")
{
    cyc cycles = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa });
    u8 acc_reg = 0x45;
    NextWord args = { .farg = 0x3, .sarg = 0x0 };

    SUBCASE("should store the accumulator in memory at the given address")
    {
        ld_Mnn_A(acc_reg, memory, args, cycles);

        CHECK_EQ(acc_reg, memory.read(0x3));
    }

    SUBCASE("should use 13 cycles")
    {
        cycles = 0;

        ld_Mnn_A(acc_reg, memory, args, cycles);

        CHECK_EQ(13, cycles);
    }
}

TEST_CASE("LR35902: LD SP, HL")
{
    cyc cycles = 0;
    u16 sp = 0;
    u8 h_reg = 0x11;
    u8 l_reg = 0x22;

    SUBCASE("should move HL into SP")
    {
        ld_sp_hl(sp, to_u16(h_reg, l_reg), cycles);

        CHECK_EQ(0x1122, sp);
    }

    SUBCASE("should use 5 cycles")
    {
        cycles = 0;

        ld_sp_hl(sp, to_u16(h_reg, l_reg), cycles);

        CHECK_EQ(6, cycles);
    }
}

TEST_CASE("LR35902: LD SP, (nn)")
{
    cyc cycles = 0;
    u16 sp = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add({ 0x65, 0x78 });
    NextWord args = {
        .farg = 0,
        .sarg = 0
    };

    SUBCASE("should move HL into SP")
    {
        ld_sp_Mnn(sp, memory, args, cycles);

        CHECK_EQ(0x7865, sp);
    }
}
}
