#include "flags.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <cstdint>

namespace emu::lr35902 {

using emu::util::byte::borrow_from;
using emu::util::byte::carried_out_of;
using emu::util::byte::is_bit_set;

Flags::Flags()
    : m_zero(false)
    , m_add_subtract(false)
    , m_half_carry(false)
    , m_carry(false)
{
}

void Flags::reset()
{
    clear_zero_flag();
    clear_half_carry_flag();
    clear_add_subtract_flag();
    clear_carry_flag();
}

u8 Flags::to_u8() const
{
    const u8 z = (m_zero ? 1 : 0) << s_zero_flag_bit_number;
    const u8 n = (m_add_subtract ? 1 : 0) << s_add_subtract_bit_number;
    const u8 h = (m_half_carry ? 1 : 0) << s_half_carry_flag_bit_number;
    const u8 c = (m_carry ? 1 : 0) << s_carry_flag_bit_number;

    return z | n | h | c | 0b0000;
}

void Flags::from_u8(u8 value)
{
    m_zero = is_bit_set(value, s_zero_flag_bit_number);
    m_add_subtract = is_bit_set(value, s_add_subtract_bit_number);
    m_half_carry = is_bit_set(value, s_half_carry_flag_bit_number);
    m_carry = is_bit_set(value, s_carry_flag_bit_number);
}

void Flags::handle_carry_flag(u8 previous, int to_add, bool cf)
{
    if (carried_out_of(s_msb, previous, to_add, cf)) {
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
}

void Flags::handle_carry_flag(u16 previous, u16 to_add)
{
    if (((previous + to_add) >> 16) & 1) { // TODO: Change to carried_out_of bit 15
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
}

void Flags::handle_borrow_flag(u8 previous, int to_subtract, bool cf)
{
    if (borrow_from(s_msb + 1, previous, to_subtract, cf)) {
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
}

void Flags::handle_borrow_flag(u16 previous, int to_subtract, bool cf)
{
    if (borrow_from(s_msb_u16 + 1, previous, to_subtract, cf)) {
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
}

void Flags::handle_half_carry_flag(u8 previous, u8 to_add, bool cf)
{
    if (carried_out_of(s_msb_first_nibble, previous, to_add, cf)) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }
}

void Flags::handle_half_carry_flag(u16 previous, u16 to_add, bool cf)
{
    if (carried_out_of(s_msb_first_nibble_u16, previous, to_add, cf)) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }
}

void Flags::handle_half_borrow_flag(u8 previous, u8 to_subtract, bool cf)
{
    if (borrow_from(s_msb_first_nibble + 1, previous, to_subtract, cf)) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }
}

void Flags::handle_zero_flag(u8 number)
{
    if (number == 0) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
}

void Flags::handle_zero_flag(u16 number)
{
    if (number == 0) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
}

void Flags::set_zero_flag()
{
    m_zero = true;
}

void Flags::clear_zero_flag()
{
    m_zero = false;
}

bool Flags::is_zero_flag_set() const
{
    return m_zero;
}

void Flags::set_carry_flag()
{
    m_carry = true;
}

void Flags::clear_carry_flag()
{
    m_carry = false;
}

bool Flags::is_carry_flag_set() const
{
    return m_carry;
}

void Flags::toggle_carry_flag()
{
    if (is_carry_flag_set()) {
        clear_carry_flag();
    } else {
        set_carry_flag();
    }
}

void Flags::set_half_carry_flag()
{
    m_half_carry = true;
}

void Flags::clear_half_carry_flag()
{
    m_half_carry = false;
}

bool Flags::is_half_carry_flag_set() const
{
    return m_half_carry;
}

void Flags::toggle_half_carry_flag()
{
    if (is_half_carry_flag_set()) {
        clear_half_carry_flag();
    } else {
        set_half_carry_flag();
    }
}

void Flags::set_add_subtract_flag()
{
    m_add_subtract = true;
}

void Flags::clear_add_subtract_flag()
{
    m_add_subtract = false;
}

bool Flags::is_add_subtract_flag_set() const
{
    return m_add_subtract;
}

TEST_CASE("LR35902: Flags")
{
    SUBCASE("should set half carry when subtracting and half borrowing")
    {
        Flags flag_reg;

        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            for (u8 value = 0; value < UINT8_MAX; ++value) {
                for (int carry = 0; carry < 2; ++carry) {
                    flag_reg.handle_half_borrow_flag(acc_reg_counter, value, carry == 1);
                    bool const expected = ((acc_reg_counter & 0xf) - (value & 0xf) - (carry & 0xf)) & 0x10;
                    CHECK_EQ(expected, flag_reg.is_half_carry_flag_set());
                }
            }
        }
    }
}
}
