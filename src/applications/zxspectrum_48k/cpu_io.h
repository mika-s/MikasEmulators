#pragma once

#include "crosscutting/typedefs.h"
#include <vector>

namespace emu::applications::zxspectrum_48k {

class CpuIo {
public:
    u8 m_out_port0xfe { 0x00 };

    std::vector<u8> m_keyboard_half_rows {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };

    u8 m_keyboard_0xfdfe { 0xff };
    u8 m_keyboard_0xfefe { 0xff };
    u8 m_keyboard_0xfbfe { 0xff };
    u8 m_keyboard_0xf7fe { 0xff };
    u8 m_keyboard_0xeffe { 0xff };
    u8 m_keyboard_0xdffe { 0xff };
    u8 m_keyboard_0xbffe { 0xff };
    u8 m_keyboard_0x7ffe { 0xff };

    u8 border_color();

    u8 keyboard_input(u16 port);

private:
    static constexpr u8 s_border_color_mask = 0b00000111;
};
}
