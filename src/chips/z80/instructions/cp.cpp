#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "chips/z80/emulator_memory.h"
#include "chips/z80/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;
    using emu::util::byte::is_bit_set;

    void cp(u8 &acc_reg, u8 value, Flags &flag_reg) {
        u8 acc_reg_copy = acc_reg;

        sub_from_register(acc_reg_copy, value, false, flag_reg);

        if (is_bit_set(value, 5)) {
            flag_reg.set_y_flag();
        } else {
            flag_reg.clear_y_flag();
        }

        if (is_bit_set(value, 3)) {
            flag_reg.set_x_flag();
        } else {
            flag_reg.clear_x_flag();
        }
    }

    /**
     * Compare register with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_r(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        cp(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Compare immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, cyc &cycles) {
        cp(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Compare memory in HL's address with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        cp(acc_reg, value, flag_reg);

        cycles = 7;
    }

    /**
     * Compare value in memory pointed to by IX or IY plus d with accumulator
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 19</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg,
                   cyc &cycles
    ) {
        cp(acc_reg, memory[ixy_reg + static_cast<i8>(args.farg)], flag_reg);

        cycles = 19;
    }

    /************************************ FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS *************************************/

    /**
     * Compare IX or IY high or low with accumulator (undocumented)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg_h_or_l, Flags &flag_reg, cyc &cycles) {
        cp(acc_reg, ixy_reg_h_or_l, flag_reg);

        cycles = 8;
    }

    /******************************** END OF FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS **********************************/

    void print_cp(std::ostream &ostream, const std::string &reg) {
        ostream << "CP "
                << reg;
    }

    void print_cp_undocumented(std::ostream &ostream, const std::string &reg) {
        ostream << "CP "
                << reg
                << "*";
    }

    void print_cp(std::ostream &ostream, const NextByte &args) {
        ostream << "CP "
                << hexify_wo_0x(args.farg);
    }

    void print_cp_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args) {
        const i8 signed_value = static_cast<i8>(args.farg);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "CP "
                << "("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }

    TEST_CASE("Z80: CP") {
        u8 acc_reg = 0;
        Flags flag_reg;

        SUBCASE("should compare the accumulator with value and set flags") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cp(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg - value) > INT8_MAX, flag_reg.is_sign_flag_set());
                    CHECK_EQ(static_cast<u8>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
                }
            }
        }
    }

    TEST_CASE("Z80: CP r") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            cp_r(acc_reg, 0, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: CP n") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            NextByte args = {.farg = 0};
            Flags flag_reg;

            cp_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: CP [HL]") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            cp_MHL(acc_reg, 0, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
