#include "chips/z80/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::z80 {

    using emu::memory::NextByte;
    using emu::util::byte::high_byte;
    using emu::util::byte::low_byte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    void adc(u8 &acc_reg, u8 value, Flags &flag_reg) {
        add_to_register(acc_reg, value, flag_reg.is_carry_flag_set(), flag_reg);
    }

    void adc(u16 &reg, u16 value, Flags &flag_reg) {
        add_to_register(reg, value, flag_reg.is_carry_flag_set(), flag_reg);
    }

    /**
     * Add from register to accumulator with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        adc(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Add immediate to accumulator with carry
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_n(u8 &acc_reg, NextByte args, Flags &flag_reg, cyc &cycles) {
        adc(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Add value in memory at HL's address to accumulator with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        adc(acc_reg, value, flag_reg);

        cycles = 7;
    }

    /**
     * Add value in memory pointed to by IX or IY plus d to accumulator with carry
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 19</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, EmulatorMemory<u16, u8> &memory, Flags &flag_reg, cyc &cycles) {
        const u16 address = ixy_reg + static_cast<i8>(args.farg);
        u8 value = memory.read(address);

        adc(acc_reg, value, flag_reg);

        memory.write(address, value);

        cycles = 19;
    }

    /**
     * Add from register to HL with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param value contains value to add to HL
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_hl_ss(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, cyc &cycles) {
        u16 hl = to_u16(h_reg, l_reg);

        adc(hl, value, flag_reg);

        h_reg = high_byte(hl);
        l_reg = low_byte(hl);

        cycles = 15;
    }

    /************************************ FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS *************************************/

    /**
     * Add from register to accumulator with carry (undocumented)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_r_undoc(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        adc(acc_reg, value, flag_reg);

        cycles = 8;
    }

    /**
     * Add from IX or IY high or low to accumulator with carry (undocumented)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param ixy_reg_h_or_l is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg_h_or_l, Flags &flag_reg, cyc &cycles) {
        adc(acc_reg, ixy_reg_h_or_l, flag_reg);

        cycles = 8;
    }

    /******************************** END OF FUNCTIONS FOR UNDOCUMENTED INSTRUCTIONS **********************************/

    void print_adc_r_s(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "ADC "
                << dest
                << ", "
                << src;
    }

    void print_adc_r_s_undocumented(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "ADC "
                << dest
                << ", "
                << src
                << "*";
    }

    void print_adc_r_n(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "ADC "
                << reg
                << ", "
                << hexify_wo_0x(args.farg);
    }

    void print_adc_MixyPn(std::ostream &ostream, const std::string &reg, const std::string &ixy_reg, const NextByte &args) {
        const i8 signed_value = static_cast<i8>(args.farg);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "ADC "
                << reg
                << ",("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }

    TEST_CASE("Z80: ADC (8-bit)") {
        u8 acc_reg = 0;

        SUBCASE("should add given value to the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    for (int carry = 0; carry < 2; ++carry) {
                        Flags flag_reg;
                        if (carry) {
                            flag_reg.set_carry_flag();
                        } else {
                            flag_reg.clear_carry_flag();
                        }

                        acc_reg = acc_reg_counter;

                        adc(acc_reg, value, flag_reg);

                        CHECK_EQ(static_cast<u8>(acc_reg_counter + value + carry), acc_reg);
                        CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                        CHECK_EQ(static_cast<i8>(acc_reg) < 0, flag_reg.is_sign_flag_set());
                        CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
                        CHECK_EQ(
                                (((acc_reg_counter & 0xf) + (value & 0xf) + (carry & 0xf)) & 0x10) > 0,
                                flag_reg.is_half_carry_flag_set()
                        );

                        const bool are_same_sign = (acc_reg_counter <= INT8_MAX && value <= INT8_MAX) || (INT8_MAX < acc_reg_counter && INT8_MAX < value);
                        const bool are_positive = are_same_sign && acc_reg_counter <= INT8_MAX;
                        const bool are_negative = are_same_sign && INT8_MAX < acc_reg_counter;
                        const bool goes_from_positive_to_negative = are_positive && acc_reg > INT8_MAX;
                        const bool goes_negative_to_positive = are_negative && acc_reg <= INT8_MAX;
                        CHECK_EQ(
                                goes_from_positive_to_negative || goes_negative_to_positive,
                                flag_reg.is_parity_overflow_flag_set()
                        );

                        // TODO: Move carry here
                    }
                }
            }
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            adc(acc_reg, 0x80, flag_reg);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x1;

            adc(acc_reg, 0x2, flag_reg);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }
    }

    TEST_CASE("Z80: ADC A, r") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            Flags flag_reg;

            adc_A_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: ADC A, n") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;
            NextByte args = {0x1};

            adc_A_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: ADC A, [HL]") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            adc_A_MHL(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
