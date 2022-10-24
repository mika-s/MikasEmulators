#include "byte_util.h"
#include "doctest.h"

namespace emu::util::byte {

    bool is_bit_set(u8 variable, unsigned int bit_position) {
        return (variable >> bit_position) & 1;
    }

    void set_bit(u8 &variable, unsigned int bit_position) {
        variable |= (1U << bit_position);
    }

    void unset_bit(u8 &variable, unsigned int bit_position) {
        variable &= ~(1U << bit_position);
    }

    u16 to_u16(u8 farg, u8 sarg) {
        return static_cast<u16>((farg << 8U) + sarg);
    }

    u32 to_u32(u8 fiarg, u8 sarg, u8 targ, u8 foarg) {
        return static_cast<u32>(fiarg << 24U | sarg << 16U | targ << 8U | foarg);
    }

    /**
     * Gets the low byte of an uint16_t.
     * <br/><br/>
     * E.g. the number 0x1234 has 0x34 as low byte.
     *
     * @param number is the uint16_t number to get the low byte of
     * @return the low byte
     */
    u8 low_byte(u16 number) {
        return (number >> (8 * 0)) & 0xff;
    }

    /**
     * Gets the high byte of an uint16_t.
     * <br/><br/>
     * E.g. the number 0x1234 has 0x12 as high byte.
     *
     * @param number is the uint16_t number to get the high byte of
     * @return the high byte
     */
    u8 high_byte(u16 number) {
        return (number >> (8 * 1)) & 0xff;
    }

    bool carried_out_of(unsigned int bit_position, u16 a, u16 b, bool cf) {
        const i32 result = a + b + (cf ? 1 : 0);
        const i32 carry = result ^ a ^ b;

        return carry & (1 << (bit_position + 1));
    }

    u8 low_nibble(u8 number) {
        return number & 0x0f;
    }

    u8 high_nibble(u8 number) {
        return number & 0xf0;
    }

    bool borrow_from(unsigned int bit_position, u16 a, u16 b, bool cf) {
        const i32 result = a - b - (cf ? 1 : 0);
        const i32 carry = result ^ a ^ b;

        return carry & (1 << bit_position);
    }

    TEST_CASE("crosscutting: byte-util") {
        constexpr unsigned int bits_in_byte = 8;

        SUBCASE("should return true if bit is set, false otherwise") {
            for (unsigned int bit_pos_to_be_set = 0; bit_pos_to_be_set < bits_in_byte; ++bit_pos_to_be_set) {
                u8 bit_field = 0;
                bit_field |= (1U << bit_pos_to_be_set);

                CHECK_EQ(true, is_bit_set(bit_field, bit_pos_to_be_set));

                for (unsigned int bit_pos = 0; bit_pos < 8; ++bit_pos) {
                    if (bit_pos != bit_pos_to_be_set) {
                        CHECK_EQ(false, is_bit_set(bit_field, bit_pos));
                    }
                }
            }
        }

        SUBCASE("should set the bit in a bit field") {
            for (unsigned int bit_pos_to_be_set = 0; bit_pos_to_be_set < bits_in_byte; ++bit_pos_to_be_set) {
                u8 bit_field = 0;
                set_bit(bit_field, bit_pos_to_be_set);

                CHECK_EQ(true, (bit_field >> bit_pos_to_be_set) & 1);

                set_bit(bit_field, bit_pos_to_be_set);

                CHECK_EQ(true, (bit_field >> bit_pos_to_be_set) & 1);   // Does not toggle

                for (unsigned int bit_pos = 0; bit_pos < 8; ++bit_pos) {
                    if (bit_pos != bit_pos_to_be_set) {
                        CHECK_EQ(false, (bit_field >> bit_pos) & 1);
                    }
                }
            }
        }

        SUBCASE("should unset the bit in a bit field") {
            for (unsigned int bit_pos_to_be_set = 0; bit_pos_to_be_set < bits_in_byte; ++bit_pos_to_be_set) {
                u8 bit_field = 0xff;
                unset_bit(bit_field, bit_pos_to_be_set);

                CHECK_EQ(false, (bit_field >> bit_pos_to_be_set) & 1);

                unset_bit(bit_field, bit_pos_to_be_set);

                CHECK_EQ(false, (bit_field >> bit_pos_to_be_set) & 1);   // Does not toggle

                for (unsigned int bit_pos = 0; bit_pos < 8; ++bit_pos) {
                    if (bit_pos != bit_pos_to_be_set) {
                        CHECK_EQ(true, (bit_field >> bit_pos) & 1);
                    }
                }
            }
        }

        SUBCASE("should return the low byte of an uint16_t") {
            CHECK_EQ(0x23, low_byte(0x0023));
            CHECK_EQ(0xed, low_byte(0xffed));
            CHECK_EQ(0x61, low_byte(0xab61));
            CHECK_EQ(0x90, low_byte(0x3290));
            CHECK_EQ(0xbc, low_byte(0xa5bc));
            CHECK_EQ(0x2f, low_byte(0x8e2f));
            CHECK_EQ(0x16, low_byte(0x5d16));
            CHECK_EQ(0x77, low_byte(0xaa77));
            CHECK_EQ(0xcd, low_byte(0x12cd));
            CHECK_EQ(0x75, low_byte(0x7675));
        }

        SUBCASE("should return the high byte of an uint16_t") {
            CHECK_EQ(0x00, high_byte(0x0023));
            CHECK_EQ(0xff, high_byte(0xffed));
            CHECK_EQ(0xab, high_byte(0xab61));
            CHECK_EQ(0x32, high_byte(0x3290));
            CHECK_EQ(0xa5, high_byte(0xa5bc));
            CHECK_EQ(0x8e, high_byte(0x8e2f));
            CHECK_EQ(0x5d, high_byte(0x5d16));
            CHECK_EQ(0xaa, high_byte(0xaa77));
            CHECK_EQ(0x12, high_byte(0x12cd));
            CHECK_EQ(0x76, high_byte(0x7675));
        }
    }
}
