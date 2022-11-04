#include "chips/z80/flags.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::z80 {

    using emu::memory::NextByte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Jump relative
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 12</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the value to add to PC
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jr(u16 &pc, const NextByte &args, cyc &cycles) {
        pc += static_cast<i8>(args.farg);

        cycles = 12;
    }

    /**
     * Jump relative if carry
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 2 or 3</li>
     *   <li>States: 7 or 12</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the value to add to PC
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jr_c(u16 &pc, const NextByte &args, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_carry_flag_set()) {
            cycles = 5;
            pc += static_cast<i8>(args.farg);
        }

        cycles += 7;
    }

    /**
     * Jump relative if no carry
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the value to add to PC
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jr_nc(u16 &pc, const NextByte &args, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_carry_flag_set()) {
            cycles = 5;
            pc += static_cast<i8>(args.farg);
        }

        cycles += 7;
    }

    /**
     * Jump relative if zero
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the value to add to PC
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jr_z(u16 &pc, const NextByte &args, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (flag_reg.is_zero_flag_set()) {
            cycles = 5;
            pc += static_cast<i8>(args.farg);
        }

        cycles += 7;
    }

    /**
     * Jump relative if not zero
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the value to add to PC
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jr_nz(u16 &pc, const NextByte &args, const Flags &flag_reg, cyc &cycles) {
        cycles = 0;

        if (!flag_reg.is_zero_flag_set()) {
            cycles = 5;
            pc += static_cast<i8>(args.farg);
        }

        cycles += 7;
    }

    void print_jr(std::ostream &ostream, const NextByte &args) {
        ostream << "JR "
                << hexify_wo_0x(static_cast<i8>(args.farg));
    }

    void print_jr(std::ostream &ostream, const std::string &condition, const NextByte &args) {
        ostream << "JR "
                << condition
                << ", "
                << hexify_wo_0x(static_cast<i8>(args.farg));
    }

    TEST_CASE("Z80: JR") {
        cyc cycles = 0;
        u16 pc = 0;
        NextByte args = {.farg = 0x11};

        SUBCASE("should jump relatively") {
            jr(pc, args, cycles);

            CHECK_EQ(args.farg, pc);
        }

        SUBCASE("should use 12 cycles") {
            cycles = 0;

            jr(pc, args, cycles);

            CHECK_EQ(12, cycles);
        }
    }

    TEST_CASE("Z80: JP C") {
        cyc cycles = 0;

        SUBCASE("should jump relatively when the carry flag is set") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jr_c(pc, args, flag_reg, cycles);

            CHECK_EQ(args.farg, pc);
        }

        SUBCASE("should not jump relatively when the carry flag is unset") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_c(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jr_c(pc, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 7 cycles when the carry flag is unset") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_c(pc, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 12 cycles when the carry flag is set") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jr_c(pc, args, flag_reg, cycles);

            CHECK_EQ(12, cycles);
        }
    }

    TEST_CASE("Z80: JR NC") {
        cyc cycles = 0;

        SUBCASE("should jump relatively when the carry flag is unset") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_nc(pc, args, flag_reg, cycles);

            CHECK_EQ(args.farg, pc);
        }

        SUBCASE("should not jump relatively when the carry flag is set") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jr_nc(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_nc(pc, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 7 cycles when the carry flag is set") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            jr_nc(pc, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 12 cycles when the carry flag is unset") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_nc(pc, args, flag_reg, cycles);

            CHECK_EQ(12, cycles);
        }
    }

    TEST_CASE("Z80: JR Z") {
        cyc cycles = 0;

        SUBCASE("should jump relatively when the zero flag is set") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_zero_flag();

            jr_z(pc, args, flag_reg, cycles);

            CHECK_EQ(args.farg, pc);
        }

        SUBCASE("should not jump relatively when the zero flag is unset") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            jr_z(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_zero_flag();

            jr_z(pc, args, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 7 cycles when the zero flag is unset") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            jr_z(pc, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 12 cycles when the zero flag is set") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_zero_flag();

            jr_z(pc, args, flag_reg, cycles);

            CHECK_EQ(12, cycles);
        }
    }

    TEST_CASE("Z80: JR NZ") {
        cyc cycles = 0;

        SUBCASE("should jump relatively when zero flag is set") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            jr_nz(pc, args, flag_reg, cycles);

            CHECK_EQ(args.farg, pc);
        }

        SUBCASE("should not jump relatively when zero flag is set") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_zero_flag();

            jr_nz(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            jr_nz(pc, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should use 7 cycles when the zero flag is set") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.set_zero_flag();

            jr_nz(pc, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 12 cycles when the zero flag is unset") {
            cycles = 0;
            u16 pc = 0;
            NextByte args = {.farg = 0x11};
            Flags flag_reg;
            flag_reg.clear_zero_flag();

            jr_nz(pc, args, flag_reg, cycles);

            CHECK_EQ(12, cycles);
        }
    }
}
