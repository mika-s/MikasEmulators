#include "shift_register.h"

namespace emu::cpu8080 {

    ShiftRegister::ShiftRegister()
            : m_value(0),
              m_offset(0) {}

    void ShiftRegister::change_offset(std::uint8_t new_offset) {
        m_offset = new_offset;
    }

    void ShiftRegister::shift(std::uint8_t shift_value) {
        std::uint16_t lsb = (m_value >> 8) & 0x00FF;
        std::uint16_t msb = shift_value << 8;
        m_value = msb | lsb;
    }

    std::uint8_t ShiftRegister::read() const {
        std::uint16_t result = m_value << m_offset;
        return (result & 0xFF00) >> 8;
    }
}
