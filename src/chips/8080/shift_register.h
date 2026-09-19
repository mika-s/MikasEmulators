#pragma once

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

class ShiftRegister {

public:
    ShiftRegister();

    void change_offset(u8 new_offset);

    void shift(u8 shift_value);

    [[nodiscard]] auto read() const -> u8;

private:
    u16 m_value;
    u8 m_offset;
};
}
