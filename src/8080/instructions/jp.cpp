#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Jump if positive
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the address to jump to
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jp(std::uint16_t &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles) {
        if (!flag_reg.is_sign_flag_set()) {
            pc = emu::util::byte::to_u16(args.sarg, args.farg);
        }

        cycles = 10;
    }

    void print_jp(std::ostream &ostream, const NextWord &args) {
        ostream << "JP "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: JP") {
        unsigned long cycles = 0;

        SUBCASE("should jump when the sign flag is unset") {
            std::uint16_t pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.clear_sign_flag();     // Positive if the sign flag is false.

            jp(pc, args, flag_reg, cycles);

            CHECK_EQ(0x2211, pc);
        }

        SUBCASE("should not jump when the sign flag is set") {
            std::uint16_t pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.set_sign_flag();

            jp(pc, args, flag_reg, cycles);

            CHECK_EQ(0, pc);
        }

        SUBCASE("should not affect any flags") {
            std::uint16_t pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.clear_sign_flag();

            jp(pc, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
            CHECK_EQ(false, flag_reg.is_sign_flag_set());
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;
            std::uint16_t pc = 0;
            NextWord args = {.farg = 0x11, .sarg = 0x22};
            Flags flag_reg;
            flag_reg.clear_sign_flag();

            jp(pc, args, flag_reg, cycles);

            CHECK_EQ(10, cycles);

            flag_reg.set_sign_flag();

            jp(pc, args, flag_reg, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
