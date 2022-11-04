#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "flags.h"
#include "instruction_util.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::z80 {

    using emu::util::byte::to_u16;

    /**
     * Return from subroutine
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret(u16 &pc, u16 &sp, const EmulatorMemory &memory, cyc &cycles) {
        execute_return(pc, sp, memory);

        cycles = 10;
    }

    /**
     * Return if carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_c(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_carry_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if no carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_nc(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_carry_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if zero
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_z(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_zero_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if not zero
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_nz(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_zero_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if minus
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_m(u16 &pc, u16 &sp, EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_sign_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if positive
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_p(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_sign_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if parity even
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_pe(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_parity_overflow_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    /**
     * Return if parity odd
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ret_po(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_parity_overflow_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    void print_ret(std::ostream &ostream) {
        ostream << "RET";
    }

    void print_ret(std::ostream &ostream, const std::string &condition) {
        ostream << "RET "
                << condition;
    }

    TEST_CASE("Z80: RET") {
        cyc cycles = 0;
        u16 pc = 0x100f;
        u16 sp = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack") {
            ret(pc, sp, memory, cycles);

            CHECK_EQ(0x01ab, pc);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            ret(pc, sp, memory, cycles);

            CHECK_EQ(10, cycles);
        }
    }

    TEST_CASE("Z80: RET C") {
        cyc cycles = 0;

        SUBCASE("should pop PC off the stack when the carry flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});
            Flags flag_reg;
            flag_reg.set_carry_flag();

            ret_c(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x01ab, pc);
        }

        SUBCASE("should not pop PC off the stack when the carry flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            ret_c(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
            Flags flag_reg;

            ret_c(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
            Flags flag_reg;
            flag_reg.set_carry_flag();

            ret_c(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET NC") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the carry flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            ret_nc(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the carry flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            ret_nc(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;

            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            ret_nc(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;

            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_nc(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET Z") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the zero flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;

            Flags flag_reg;
            flag_reg.set_zero_flag();

            ret_z(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
            CHECK_EQ(0x2, sp);
        }

        SUBCASE("should not pop PC off the stack when the zero flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            ret_z(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
            CHECK_EQ(0, sp);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_z(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_zero_flag();

            ret_z(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET NZ") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the zero flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            ret_nz(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the zero flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_zero_flag();

            ret_nz(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_zero_flag();

            ret_nz(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_nz(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET M") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the sign flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            ret_m(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the sign flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_sign_flag();

            ret_m(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_m(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            ret_m(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET P") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the sign flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_sign_flag();

            ret_p(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the sign flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            ret_p(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            ret_p(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_p(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET PE") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the parity flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_parity_overflow_flag();

            ret_pe(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the parity flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_parity_overflow_flag();

            ret_pe(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_pe(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_parity_overflow_flag();

            ret_pe(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: RET PO") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the parity flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_parity_overflow_flag();

            ret_po(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        }

        SUBCASE("should not pop PC off the stack when the parity flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_parity_overflow_flag();

            ret_po(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;

            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_parity_overflow_flag();

            ret_po(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            ret_po(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
