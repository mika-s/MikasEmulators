#include "chips/8080/flags.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::i8080 {

    using emu::memory::NextByte;
    using emu::util::string::hexify_wo_0x;

    /**
     * Compare immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cpi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, cyc &cycles) {
        const u16 new_acc_reg = acc_reg - args.farg;

        flag_reg.handle_borrow_flag(acc_reg, args.farg, false);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_aux_borrow_flag(acc_reg, args.farg, false);

        cycles = 7;
    }

    void print_cpi(std::ostream &ostream, const NextByte &args) {
        ostream << "CPI "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: CPI") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should compare the accumulator with value and set flags") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {.farg = value};
                    acc_reg = acc_reg_counter;

                    cpi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg - value) > INT8_MAX, flag_reg.is_sign_flag_set());
                    CHECK_EQ(static_cast<u8>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            NextByte args = {.farg = 0};
            Flags flag_reg;

            cpi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
