#include <iostream>
#include "doctest.h"
#include "z80/instructions/instructions.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    void ld(u8 &to, u8 value) {
        to = value;
    }

    /**
     * Move from register to register
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
    void ld_r_r(u8 &to, u8 value, unsigned long &cycles) {
        ld(to, value);

        cycles = 4;
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
     * @param to is the register to load into, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ld_r_n(u8 &to, const NextByte &args, unsigned long &cycles) {
        ld(to, args.farg);

        cycles = 7;
    }

    /**
     * Move memory in HL's address to register
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
    void ld_r_MHL(u8 &to, u8 value, unsigned long &cycles) {
        ld(to, value);

        cycles = 7;
    }

    /**
     * Move register to memory in HL's address
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
    void ld_MHL_r(u8 &to, u8 value, unsigned long &cycles) {
        ld(to, value);

        cycles = 7;
    }

    /**
     * Move immediate to memory in HL's address
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg is the register to load into, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void ld_MHL_n(u8 &reg, const NextByte &args, unsigned long &cycles) {
        reg = args.farg;

        cycles = 10;
    }

    /**
     * Move from memory to register
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
    void ld_A_Mss(u8 &to, u8 value, unsigned long &cycles) {
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
    void ld_A_Mnn(u8 &acc_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        acc_reg = memory[to_u16(args.sarg, args.farg)];

        cycles = 13;
    }

    /**
     * Move register to memory in HL's address
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
    void ld_Mss_A(u8 &to, u8 acc_reg, unsigned long &cycles) {
        ld(to, acc_reg);

        cycles = 7;
    }

    /**
     * Move memory in address nn to accumulator
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
    void ld_Mnn_A(u8 &acc_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        const u16 address = to_u16(args.sarg, args.farg);

        memory[address] = acc_reg;

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
     * @param l_reg is the L register, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param memory is the memory
     * @param args contains the argument with the address to the wanted value in memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ld_HL_Mnn(u8 &l_reg, u8 &h_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        l_reg = memory[to_u16(args.sarg, args.farg)];
        h_reg = memory[to_u16(args.sarg, args.farg) + 1];

        cycles = 16;
    }

    /**
     * Load register pair immediate
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
    void ld_dd_nn(u8 &reg1, u8 &reg2, const NextWord &args, unsigned long &cycles) {
        reg1 = args.sarg;
        reg2 = args.farg;

        cycles = 10;
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
    void ld_sp_nn(u16 &sp, const NextWord &args, unsigned long &cycles) {
        sp = to_u16(args.sarg, args.farg);

        cycles = 10;
    }

    /**
     * Move content of HL register to SP
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
    void ld_sp_hl(u16 &sp, u16 hl, unsigned long &cycles) {
        sp = hl;

        cycles = 6;
    }

    /**
     * Move content of IX/IY register to SP
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param ixy is the value to store in SP
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ld_sp_ixy(u16 &sp, u16 ixy, unsigned long &cycles) {
        sp = ixy;

        cycles = 10;
    }

    /**
     * Store H and L direct
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
    void ld_Mnn_HL(u8 l_reg, u8 h_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        const u16 l_address = to_u16(args.sarg, args.farg);
        const u16 h_address = l_address + 1;

        memory[l_address] = l_reg;
        memory[h_address] = h_reg;

        cycles = 16;
    }

    /**
     * Store reg1 and reg2 direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 20</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to store
     * @param reg2 is the second register to store
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ld_Mnn_dd(u8 reg1, u8 reg2, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        const u16 first_address = to_u16(args.sarg, args.farg);
        const u16 second_address = first_address + 1;

        memory[first_address] = reg1;
        memory[second_address] = reg2;

        cycles = 20;
    }

    void print_ld(std::ostream &ostream, const std::string &reg1, const std::string &reg2) {
        ostream << "LD "
                << reg1
                << ","
                << reg2;
    }

    void print_ld(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "LD "
                << reg
                << ","
                << hexify_wo_0x(args.farg);
    }

    void print_ld(std::ostream &ostream, const std::string &reg, const NextWord &args) {
        ostream << "LD "
                << reg
                << ", ("
                << hexify_wo_0x(to_u16(args.farg, args.sarg))
                << ")";
    }

    void print_ld_dd_nn(std::ostream &ostream, const std::string &reg, const NextWord &args) {
        ostream << "LD "
                << reg
                << ","
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    void print_ld_Mnn_HL(std::ostream &ostream, const NextWord &args) {
        ostream << "LD ("
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg)
                << "), HL";
    }

    void print_ld_Mnn_A(std::ostream &ostream, const NextWord &args) {
        ostream << "LD ("
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg)
                << "), A";
    }

    TEST_CASE("Z80: LD") {
        SUBCASE("should load register/memory with value from register/memory") {
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

    TEST_CASE("Z80: LD r, r'") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            u8 reg1 = 1;
            u8 reg2 = 2;

            ld_r_r(reg1, reg2, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: LD r, n") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            u8 reg = 0;
            NextByte args = {.farg = 0x11};

            ld_r_n(reg, args, cycles);

            CHECK_EQ(7, cycles);
        }

    }

    TEST_CASE("Z80: LD (HL), r") {
        unsigned long cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        u8 acc_reg = 0;
        u8 reg1 = 0x0;
        u8 reg2 = 0x3;

        SUBCASE("should store the accumulator in memory at the given address") {
            ld_MHL_r(memory[to_u16(reg1, reg2)], acc_reg, cycles);

            CHECK_EQ(acc_reg, memory[0x3]);
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;

            ld_MHL_r(memory[to_u16(reg1, reg2)], acc_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: LD A, (nn)") {
        unsigned long cycles = 0;
        u8 reg = 0xe;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x04, .sarg = 0};

        SUBCASE("should load the register from memory using the address in args") {
            ld_A_Mnn(reg, memory, args, cycles);

            CHECK_EQ(memory[0x04], reg);
        }

        SUBCASE("should use 13 cycles") {
            cycles = 0;

            ld_A_Mnn(reg, memory, args, cycles);

            CHECK_EQ(13, cycles);
        }
    }

    TEST_CASE("Z80: LD HL, (nn)") {
        unsigned long cycles = 0;
        u8 l_reg = 0xe;
        u8 h_reg = 0x42;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x04, .sarg = 0};

        SUBCASE("should load HL from memory using the address in args") {
            ld_HL_Mnn(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(memory[0x04], l_reg);
            CHECK_EQ(memory[0x05], h_reg);
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;

            ld_HL_Mnn(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(16, cycles);
        }
    }

    TEST_CASE("Z80: LD dd, nn") {
        unsigned long cycles = 0;
        u16 sp = 0xe;
        u8 reg1 = 0xe;
        u8 reg2 = 0;
        NextWord args = {.farg = 0x12, .sarg = 0x3a};

        SUBCASE("should load immediate into register pair") {
            ld_dd_nn(reg1, reg2, args, cycles);

            CHECK_EQ(args.sarg, reg1);
            CHECK_EQ(args.farg, reg2);
        }

        SUBCASE("should load immediate into SP") {
            ld_sp_nn(sp, args, cycles);

            CHECK_EQ(to_u16(args.sarg, args.farg), sp);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            ld_dd_nn(reg1, reg2, args, cycles);

            CHECK_EQ(10, cycles);
        }
    }

    TEST_CASE("Z80: LD (nn), HL") {
        unsigned long cycles = 0;
        u8 l_reg = 0x22;
        u8 h_reg = 0x11;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0xff, 0xaa, 0xbb, 0xcc, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x2, .sarg = 0x0};

        SUBCASE("should load memory with H and L registers") {
            ld_Mnn_HL(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(l_reg, memory[0x02]);
            CHECK_EQ(h_reg, memory[0x03]);
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;

            ld_Mnn_HL(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(16, cycles);
        }
    }

    TEST_CASE("Z80: LD (nn), A") {
        unsigned long cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        u8 acc_reg = 0x45;
        NextWord args = {.farg = 0x3, .sarg = 0x0};

        SUBCASE("should store the accumulator in memory at the given address") {
            ld_Mnn_A(acc_reg, memory, args, cycles);

            CHECK_EQ(acc_reg, memory[0x3]);
        }

        SUBCASE("should use 13 cycles") {
            cycles = 0;

            ld_Mnn_A(acc_reg, memory, args, cycles);

            CHECK_EQ(13, cycles);
        }
    }

    TEST_CASE("Z80: LD SP, HL") {
        unsigned long cycles = 0;
        u16 sp = 0;
        u8 h_reg = 0x11;
        u8 l_reg = 0x22;

        SUBCASE("should move HL into SP") {
            ld_sp_hl(sp, to_u16(h_reg, l_reg), cycles);

            CHECK_EQ(0x1122, sp);
        }

        SUBCASE("should use 5 cycles") {
            cycles = 0;

            ld_sp_hl(sp, to_u16(h_reg, l_reg), cycles);

            CHECK_EQ(6, cycles);
        }
    }
}
