#pragma once

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

class ShiftRegister {

public:
    ShiftRegister();

    void change_offset(u8 new_offset);

    void shift(u8 shift_value);

    [[nodiscard]] u8 read() const;

private:
    u16 m_value;
    u8 m_offset;
};
}
