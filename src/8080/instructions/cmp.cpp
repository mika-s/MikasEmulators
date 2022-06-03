#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Compare with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cmp(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles) {
        cmp(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Compare with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void cmp(std::uint8_t &acc_reg, std::uint8_t value, Flags &flag_reg, unsigned long &cycles,
             bool is_memory_involved) {
        const std::uint8_t previous = acc_reg;
        const std::uint8_t new_acc_reg = previous - value;

        flag_reg.handle_borrow_flag(previous, value);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_aux_borrow_flag(previous, value, false);

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_cmp(std::ostream &ostream, const std::string &reg) {
        ostream << "CMP " << reg;
    }

    TEST_CASE("8080: CMP") {
        unsigned long cycles = 0;
        std::uint8_t acc_reg = 0;
        Flags flag_reg;

        SUBCASE("should compare the accumulator with value and set flags") {

            for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cmp(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(static_cast<std::uint8_t>(acc_reg - value) > 127, flag_reg.is_sign_flag_set());
                    CHECK_EQ(static_cast<std::uint8_t>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            std::uint8_t value = 0;

            cmp(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            std::uint8_t value = 0;

            cmp(acc_reg, value, flag_reg, cycles, true);

            CHECK_EQ(7, cycles);
        }
    }
}
