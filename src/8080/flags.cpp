#include <cstdint>
#include "8080/flags.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {

    using emu::util::byte::is_bit_set;

    Flags::Flags() : carry(false), parity(false), auxiliary_carry(false), zero(false), sign(false) {}

    void Flags::reset() {
        clear_carry_flag();
        clear_parity_flag();
        clear_aux_carry_flag();
        clear_zero_flag();
        clear_sign_flag();
    }

    std::uint8_t Flags::to_uint8_t() const {
        const std::uint8_t s = (sign ? 1 : 0) << 7;
        const std::uint8_t z = (zero ? 1 : 0) << 6;
        const std::uint8_t unused1 = 0 << 5;
        const std::uint8_t ac = (auxiliary_carry ? 1 : 0) << 4;
        const std::uint8_t unused2 = 0 << 3;
        const std::uint8_t p = (parity ? 1 : 0) << 2;
        const std::uint8_t unused3 = 1 << 1;
        const std::uint8_t c = (carry ? 1 : 0) << 0;

        return s | z | unused1 | ac | unused2 | p | unused3 | c;
    }

    void Flags::from_uint8_t(std::uint8_t value) {
        sign = is_bit_set(value, 7);
        zero = is_bit_set(value, 6);
        auxiliary_carry = is_bit_set(value, 4);
        parity = is_bit_set(value, 2);
        carry = is_bit_set(value, 0);
    }

    void Flags::handle_carry_flag(std::uint8_t previous, int value_to_add) {
        int new_value = previous + value_to_add;
        if (new_value > 255) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_carry_flag_dad(std::uint16_t previous, std::uint16_t value_to_add) {
        if (((previous + value_to_add) >> 16) & 1) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_borrow_flag(std::uint8_t previous, int value_to_subtract) {
        if (previous < value_to_subtract) {
            set_carry_flag();
        } else {
            clear_carry_flag();
        }
    }

    void Flags::handle_aux_carry_flag(std::uint8_t previous, std::uint8_t value_to_add, bool cf) {
        const std::uint8_t result = previous + value_to_add + (cf ? 1 : 0);
        const std::uint8_t half_carry = (previous ^ value_to_add ^ result) & 0x10;

        if (half_carry > 0) {
            set_aux_carry_flag();
        } else {
            clear_aux_carry_flag();
        }
    }

    void Flags::handle_aux_borrow_flag(std::uint8_t previous, std::uint8_t value_to_subtract, bool cf) {
        const std::uint8_t result = previous - value_to_subtract - (cf ? 1 : 0);
        const std::uint8_t half_carry = ~(previous ^ value_to_subtract ^ result) & 0x10;

        if (half_carry > 0) {
            set_aux_carry_flag();
        } else {
            clear_aux_carry_flag();
        }
    }

    void Flags::handle_zero_flag(std::uint8_t number) {
        if (number == 0) {
            set_zero_flag();
        } else {
            clear_zero_flag();
        }
    }

    void Flags::handle_parity_flag(std::uint8_t number) {
        if (should_parity_flag_be_set(number)) {
            set_parity_flag();
        } else {
            clear_parity_flag();
        }
    }

    void Flags::handle_sign_flag(std::uint8_t number) {
        if (number > 127) {
            set_sign_flag();
        } else {
            clear_sign_flag();
        }
    }

    bool Flags::should_parity_flag_be_set(std::uint8_t number) {
        bool isOdd = false;
        std::uint8_t copy = number;
        while (copy) {
            isOdd = !isOdd;
            copy = copy & (copy - 1u);
        }

        return !isOdd;
    }

    void Flags::set_zero_flag() {
        zero = true;
    }

    void Flags::clear_zero_flag() {
        zero = false;
    }

    bool Flags::is_zero_flag_set() const {
        return zero;
    }

    void Flags::set_carry_flag() {
        carry = true;
    }

    void Flags::clear_carry_flag() {
        carry = false;
    }

    bool Flags::is_carry_flag_set() const {
        return carry;
    }

    void Flags::toggle_carry_flag() {
        if (is_carry_flag_set()) {
            clear_carry_flag();
        } else {
            set_carry_flag();
        }
    }

    void Flags::set_aux_carry_flag() {
        auxiliary_carry = true;
    }

    void Flags::clear_aux_carry_flag() {
        auxiliary_carry = false;
    }

    bool Flags::is_aux_carry_flag_set() const {
        return auxiliary_carry;
    }

    void Flags::set_sign_flag() {
        sign = true;
    }

    void Flags::clear_sign_flag() {
        sign = false;
    }

    bool Flags::is_sign_flag_set() const {
        return sign;
    }

    void Flags::set_parity_flag() {
        parity = true;
    }

    void Flags::clear_parity_flag() {
        parity = false;
    }

    bool Flags::is_parity_flag_set() const {
        return parity;
    }
}
