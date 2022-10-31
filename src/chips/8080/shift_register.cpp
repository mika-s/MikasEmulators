#include "shift_register.h"

namespace emu::i8080 {

    ShiftRegister::ShiftRegister()
        : m_value(0),
          m_offset(0) {}

    void ShiftRegister::change_offset(u8 new_offset) {
        m_offset = new_offset & 0b111;
    }

    void ShiftRegister::shift(u8 shift_value) {
        u16 lsb = (m_value >> 8) & 0x00ff;
        u16 msb = shift_value << 8;
        m_value = msb | lsb;
    }

    u8 ShiftRegister::read() const {
        u16 result = m_value << m_offset;
        return (result & 0xff00) >> 8;
    }
}
