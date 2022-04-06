#include <cstdint>
#include <gtest/gtest.h>
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080::tests {
    TEST(DadTest, ShouldAddGivenValueToHL) {
        unsigned long cycles = 0;

        for (std::uint16_t hl_counter = 0; hl_counter < 100; ++hl_counter) {
            for (std::uint16_t value_to_add = 0; value_to_add < UINT16_MAX; ++value_to_add) {
                Flags flag_reg;

                std::uint8_t h_reg = emu::util::byte::second_byte(hl_counter);
                std::uint8_t l_reg = emu::util::byte::first_byte(hl_counter);

                dad(h_reg, l_reg, value_to_add, flag_reg, cycles);
                EXPECT_EQ(
                        static_cast<std::uint16_t>(hl_counter + value_to_add),
                        emu::util::byte::to_u16(h_reg, l_reg)
                );
            }
        }
    }

    TEST(DadTest, ShouldSetCarryFlagWhenCarriedOutOfMsb) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t h_reg = 0xff;
        std::uint8_t l_reg = 0xff;
        std::uint16_t value_to_add = 0x1;
        dad(h_reg, l_reg, value_to_add, flag_reg, cycles);
        EXPECT_EQ(true, flag_reg.is_carry_flag_set());
    }

    TEST(DadTest, ShouldUse10Cycles) {
        unsigned long cycles = 0;
        Flags flag_reg;

        std::uint8_t h_reg = 0;
        std::uint8_t l_reg = 0xE;
        std::uint16_t value_to_add = 0x4;
        dad(h_reg, l_reg, value_to_add, flag_reg, cycles);
        EXPECT_EQ(10, cycles);
    }
}
