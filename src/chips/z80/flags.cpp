#include "doctest.h"
#include "flags.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::borrow_from;
    using emu::util::byte::carried_out_of;
    using emu::util::byte::is_bit_set;

    Flags::Flags()
            : m_zero(false),
              m_sign(false),
              m_y_unused_flag(false),
              m_half_carry(false),
              m_x_unused_flag(false),
              m_parity_overflow(false),
              m_add_subtract(false),
              m_carry(false) {}

    void Flags::reset() {
        clear_sign_flag();
        clear_zero_flag();
        clear_y_flag();
        clear_half_carry_flag();
        clear_x_flag();
        clear_parity_overflow_flag();
        clear_add_subtract_flag();
        clear_carry_flag();
    }

    u8 Flags::to_u8() const {
        const u8 s = (m_sign ? 1 : 0) << sign_flag_bit_number;
        const u8 z = (m_zero ? 1 : 0) << zero_flag_bit_number;
        const u8 y = (m_y_unused_flag ? 1 : 0) << y_flag_bit_number;
        const u8 h = (m_half_carry ? 1 : 0) << half_carry_flag_bit_number;
        const u8 x = (m_x_unused_flag ? 1 : 0) << x_flag_bit_number;
        const u8 pv = (m_parity_overflow ? 1 : 0) << parity_overflow_flag_bit_number;
        const u8 n = (m_add_subtract ? 1 : 0) << add_subtract_bit_number;
        const u8 c = (m_carry ? 1 : 0) << carry_flag_bit_number;

        return s | z | y | h | x | pv | n | c;
    }

    void Flags::from_u8(u8 value) {
        m_sign = is_bit_set(value, sign_flag_bit_number);
        m_zero = is_bit_set(value, zero_flag_bit_number);
        m_y_unused_flag = is_bit_set(value, y_flag_bit_number);
        m_half_carry = is_bit_set(value, half_carry_flag_bit_number);
        m_x_unused_flag = is_bit_set(value, x_flag_bit_number);
        m_parity_overflow = is_bit_set(value, parity_overflow_flag_bit_number);
        m_add_subtract = is_bit_set(value, add_subtract_bit_number);
        m_carry = is_bit_set(value, carry_flag_bit_number);
    }

    void Flags::handle_carry_flag(u8 previous, int to_add, bool cf) {
        if (carried_out_of(msb, previous, to_add, cf)) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_carry_flag(u16 previous, u16 to_add) {
        if (((previous + to_add) >> 16) & 1) {  // TODO: Change to carried_out_of bit 15
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_borrow_flag(u8 previous, int to_subtract, bool cf) {
        if (borrow_from(msb + 1, previous, to_subtract, cf)) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_borrow_flag(u16 previous, int to_subtract, bool cf) {
        if (borrow_from(msb_u16 + 1, previous, to_subtract, cf)) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_half_carry_flag(u8 previous, u8 to_add, bool cf) {
        if (carried_out_of(msb_first_nibble, previous, to_add, cf)) {
            set_half_carry_flag();
        } else {
            clear_half_carry_flag();
        }
    }

    void Flags::handle_half_carry_flag(u16 previous, u16 to_add, bool cf) {
        if (carried_out_of(msb_first_nibble_u16, previous, to_add, cf)) {
            set_half_carry_flag();
        } else {
            clear_half_carry_flag();
        }
    }

    void Flags::handle_half_borrow_flag(u8 previous, u8 to_subtract, bool cf) {
        if (borrow_from(msb_first_nibble + 1, previous, to_subtract, cf)) {
            set_half_carry_flag();
        } else {
            clear_half_carry_flag();
        }
    }

    void Flags::handle_zero_flag(u8 number) {
        if (number == 0) {
            set_zero_flag();
        } else {
            clear_zero_flag();
        }
    }

    void Flags::handle_zero_flag(u16 number) {
        if (number == 0) {
            set_zero_flag();
        } else {
            clear_zero_flag();
        }
    }

    void Flags::handle_parity_flag(u8 number) {
        if (should_parity_flag_be_set(number)) {
            set_parity_overflow_flag();
        } else {
            clear_parity_overflow_flag();
        }
    }

    void Flags::handle_overflow_flag(u8 previous, u8 to_add, bool cf) {
        if (should_overflow_flag_be_set(previous, to_add, cf)) {
            set_parity_overflow_flag();
        } else {
            clear_parity_overflow_flag();
        }
    }

    void Flags::handle_sign_flag(u8 number) {
        if (number > INT8_MAX) {
            set_sign_flag();
        } else {
            clear_sign_flag();
        }
    }

    void Flags::handle_sign_flag(u16 number) {
        if (static_cast<i16>(number) < 0) {
            set_sign_flag();
        } else {
            clear_sign_flag();
        }
    }

    void Flags::handle_xy_flags(u8 number) {
        if (is_bit_set(number, y_flag_bit_number)) {
            set_y_flag();
        } else {
            clear_y_flag();
        }

        if (is_bit_set(number, x_flag_bit_number)) {
            set_x_flag();
        } else {
            clear_x_flag();
        }
    }

    bool Flags::should_parity_flag_be_set(u8 number) {
        bool isOdd = false;
        u8 copy = number;
        while (copy) {
            isOdd = !isOdd;
            copy = copy & (copy - 1u);
        }

        return !isOdd;
    }

    bool Flags::should_overflow_flag_be_set(u8 previous, u8 to_add, bool cf) {
        return carried_out_of(msb, previous, to_add, cf) != carried_out_of(msb - 1, previous, to_add, cf);
    }

    void Flags::set_zero_flag() {
        m_zero = true;
    }

    void Flags::clear_zero_flag() {
        m_zero = false;
    }

    bool Flags::is_zero_flag_set() const {
        return m_zero;
    }

    void Flags::set_carry_flag() {
        m_carry = true;
    }

    void Flags::clear_carry_flag() {
        m_carry = false;
    }

    bool Flags::is_carry_flag_set() const {
        return m_carry;
    }

    void Flags::toggle_carry_flag() {
        if (is_carry_flag_set()) {
            clear_carry_flag();
        } else {
            set_carry_flag();
        }
    }

    void Flags::set_half_carry_flag() {
        m_half_carry = true;
    }

    void Flags::clear_half_carry_flag() {
        m_half_carry = false;
    }

    bool Flags::is_half_carry_flag_set() const {
        return m_half_carry;
    }

    void Flags::toggle_half_carry_flag() {
        if (is_half_carry_flag_set()) {
            clear_half_carry_flag();
        } else {
            set_half_carry_flag();
        }
    }

    void Flags::set_add_subtract_flag() {
        m_add_subtract = true;
    }

    void Flags::clear_add_subtract_flag() {
        m_add_subtract = false;
    }

    bool Flags::is_add_subtract_flag_set() const {
        return m_add_subtract;
    }

    void Flags::set_sign_flag() {
        m_sign = true;
    }

    void Flags::clear_sign_flag() {
        m_sign = false;
    }

    bool Flags::is_sign_flag_set() const {
        return m_sign;
    }

    void Flags::set_parity_overflow_flag() {
        m_parity_overflow = true;
    }

    void Flags::clear_parity_overflow_flag() {
        m_parity_overflow = false;
    }

    bool Flags::is_parity_overflow_flag_set() const {
        return m_parity_overflow;
    }

    bool Flags::is_y_flag_set() const {
        return m_y_unused_flag;
    }

    void Flags::set_y_flag() {
        m_y_unused_flag = true;
    }

    void Flags::clear_y_flag() {
        m_y_unused_flag = false;
    }

    bool Flags::is_x_flag_set() const {
        return m_x_unused_flag;
    }

    void Flags::set_x_flag() {
        m_x_unused_flag = true;
    }

    void Flags::clear_x_flag() {
        m_x_unused_flag = false;
    }

    TEST_CASE("Z80: Flags") {
        SUBCASE("should set half carry when subtracting and half borrowing") {
            Flags flag_reg;

            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    for (int carry = 0; carry < 2; ++carry) {
                        flag_reg.handle_half_borrow_flag(acc_reg_counter, value, carry == 1);
                        const bool expected = ((acc_reg_counter & 0xf) - (value & 0xf) - (carry & 0xf)) & 0x10;
                        CHECK_EQ(expected, flag_reg.is_half_carry_flag_set());
                    }
                }
            }
        }
    }
}
