#pragma once

#include "crosscutting/typedefs.h"

namespace emu::lr35902 {

class Flags {
public:
    Flags();

    void reset();

    [[nodiscard]] u8 to_u8() const;

    void from_u8(u8 value);

    void handle_zero_flag(u8 number);

    void handle_zero_flag(u16 number);

    void handle_carry_flag(u8 previous, int to_add, bool cf);

    void handle_carry_flag(u16 previous, u16 to_add);

    void handle_borrow_flag(u8 previous, int to_subtract, bool cf);

    void handle_half_carry_flag(u8 previous, u8 to_add, bool cf);

    void handle_half_carry_flag(u16 previous, u16 to_add, bool cf);

    void handle_half_borrow_flag(u8 previous, u8 to_subtract, bool cf);

    void handle_borrow_flag(u16 previous, int to_subtract, bool cf);

    void set_zero_flag();

    void clear_zero_flag();

    [[nodiscard]] bool is_zero_flag_set() const;

    void set_carry_flag();

    void clear_carry_flag();

    [[nodiscard]] bool is_carry_flag_set() const;

    void toggle_carry_flag();

    void set_half_carry_flag();

    void clear_half_carry_flag();

    [[nodiscard]] bool is_half_carry_flag_set() const;

    void toggle_half_carry_flag();

    void set_add_subtract_flag();

    void clear_add_subtract_flag();

    [[nodiscard]] bool is_add_subtract_flag_set() const;

private:
    static constexpr unsigned int s_msb_u16 = 15;
    static constexpr unsigned int s_msb = 7;
    static constexpr unsigned int s_msb_first_nibble = 3;
    static constexpr unsigned int s_msb_first_nibble_u16 = 11;

    static constexpr unsigned int s_carry_flag_bit_number = 4;
    static constexpr unsigned int s_half_carry_flag_bit_number = 5;
    static constexpr unsigned int s_add_subtract_bit_number = 6;
    static constexpr unsigned int s_zero_flag_bit_number = 7;

    bool m_zero;
    bool m_add_subtract;
    bool m_half_carry;
    bool m_carry;
};
}
