#include "8080/flags.h"
#include "crosscutting/util/byte_util.h"

namespace emu::i8080 {

    using emu::util::byte::borrowed_out_of;
    using emu::util::byte::carried_out_of;
    using emu::util::byte::is_bit_set;

    Flags::Flags()
            : m_carry(false),
              m_parity(false),
              m_auxiliary_carry(false),
              m_zero(false),
              m_sign(false) {}

    void Flags::reset() {
        clear_carry_flag();
        clear_parity_flag();
        clear_aux_carry_flag();
        clear_zero_flag();
        clear_sign_flag();
    }

    u8 Flags::to_u8() const {
        const u8 s = (m_sign ? 1 : 0) << 7;
        const u8 z = (m_zero ? 1 : 0) << 6;
        const u8 unused1 = 0 << 5;
        const u8 ac = (m_auxiliary_carry ? 1 : 0) << 4;
        const u8 unused2 = 0 << 3;
        const u8 p = (m_parity ? 1 : 0) << 2;
        const u8 unused3 = 1 << 1;
        const u8 c = (m_carry ? 1 : 0) << 0;

        return s | z | unused1 | ac | unused2 | p | unused3 | c;
    }

    void Flags::from_u8(u8 value) {
        m_sign = is_bit_set(value, 7);
        m_zero = is_bit_set(value, 6);
        m_auxiliary_carry = is_bit_set(value, 4);
        m_parity = is_bit_set(value, 2);
        m_carry = is_bit_set(value, 0);
    }

    void Flags::handle_carry_flag(u8 previous, int value_to_add, bool cf) {
        if (carried_out_of(msb, previous, value_to_add, cf)) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_carry_flag_dad(u16 previous, u16 value_to_add) {
        if (((previous + value_to_add) >> 16) & 1) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_borrow_flag(u8 previous, int value_to_subtract, bool cf) {
        if (borrowed_out_of(msb, previous, value_to_subtract, cf)) {
            clear_carry_flag();
        } else {
            set_carry_flag();
        }
    }

    void Flags::handle_aux_carry_flag(u8 previous, u8 value_to_add, bool cf) {
        if (carried_out_of(msb_first_nibble, previous, value_to_add, cf)) {
            set_aux_carry_flag();
        } else {
            clear_aux_carry_flag();
        }
    }

    void Flags::handle_aux_borrow_flag(u8 previous, u8 value_to_subtract, bool cf) {
        if (borrowed_out_of(msb_first_nibble, previous, value_to_subtract, cf)) {
            set_aux_carry_flag();
        } else {
            clear_aux_carry_flag();
        }
    }

    void Flags::handle_zero_flag(u8 number) {
        if (number == 0) {
            set_zero_flag();
        } else {
            clear_zero_flag();
        }
    }

    void Flags::handle_parity_flag(u8 number) {
        if (should_parity_flag_be_set(number)) {
            set_parity_flag();
        } else {
            clear_parity_flag();
        }
    }

    void Flags::handle_sign_flag(u8 number) {
        if (number > 127) {
            set_sign_flag();
        } else {
            clear_sign_flag();
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

    void Flags::set_aux_carry_flag() {
        m_auxiliary_carry = true;
    }

    void Flags::clear_aux_carry_flag() {
        m_auxiliary_carry = false;
    }

    bool Flags::is_aux_carry_flag_set() const {
        return m_auxiliary_carry;
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

    void Flags::set_parity_flag() {
        m_parity = true;
    }

    void Flags::clear_parity_flag() {
        m_parity = false;
    }

    bool Flags::is_parity_flag_set() const {
        return m_parity;
    }
}
