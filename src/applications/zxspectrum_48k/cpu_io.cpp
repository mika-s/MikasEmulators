#include "cpu_io.h"

namespace emu::applications::zxspectrum_48k {

u8 CpuIo::border_color()
{
    return m_out_port0xfe & s_border_color_mask;
}

u8 CpuIo::keyboard_input(u16 port) const
{
    return m_keyboard.at(port);
}
}
