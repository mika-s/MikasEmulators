#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::zxspectrum_48k {

class CpuIo {
public:
    u8 m_in_port0xfe;
    u8 m_out_port0xfe;

    u8 border_color();

private:

    static constexpr u8 s_border_color_mask = 0b00000111;
};
}
