#include "shift_register.h"

namespace emu::i8080 {

ShiftRegister::ShiftRegister()
    : m_value(0)
    , m_offset(0)
{
}

void ShiftRegister::change_offset(const u8 new_offset)
{
    m_offset = new_offset & 0b111;
}

void ShiftRegister::shift(const u8 shift_value)
{
    u16 const lsb = (m_value >> 8) & 0x00ff;
    u16 const msb = shift_value << 8;
    m_value = msb | lsb;
}

auto ShiftRegister::read() const -> u8
{
    u16 const result = m_value << m_offset;
    return (result & 0xff00) >> 8;
}
}
