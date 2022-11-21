#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;

    /**
     * Pop
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg1 is the first register to pop to, which will be mutated
     * @param reg2 is the second register to pop to, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop(u8 &reg1, u8 &reg2, u16 &sp, const EmulatorMemory<u16, u8> &memory, cyc &cycles) {
        reg2 = memory.read(sp++);
        reg1 = memory.read(sp++);

        cycles = 10;
    }

    /**
     * Pop
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param flag_reg is the flag register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop_psw(Flags &flag_reg, u8 &acc_reg, u16 &sp, const EmulatorMemory<u16, u8> &memory, cyc &cycles) {
        flag_reg.from_u8(memory.read(sp++));
        acc_reg = memory.read(sp++);

        cycles = 10;
    }

    void print_pop(std::ostream &ostream, const std::string &reg) {
        ostream << "POP "
                << reg;
    }

    TEST_CASE("8080: POP") {
        cyc cycles = 0;

        EmulatorMemory<u16, u8> memory;
        memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0xff, 0x05, 0x06, 0x07, 0x08});

        SUBCASE("should pop register from stack") {
            u8 reg1 = 0xaa;
            u8 reg2 = 0xbb;
            u16 sp = 0x03;

            pop(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(memory.read(0x03), reg2);
            CHECK_EQ(memory.read(0x04), reg1);
            CHECK_EQ(0x05, sp);
        }

        SUBCASE("should pop PSW from the stack") {
            Flags flag_reg;
            u8 acc_reg = 0xaa;
            u16 sp = 0x03;

            pop_psw(flag_reg, acc_reg, sp, memory, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_zero_flag_set());
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_sign_flag_set());
            CHECK_EQ(0x05, acc_reg);
            CHECK_EQ(0x05, sp);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            u8 reg1 = 0;
            u8 reg2 = 0;
            u16 sp = 0x03;

            pop(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
