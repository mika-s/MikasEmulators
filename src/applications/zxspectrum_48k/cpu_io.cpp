#include "cpu_io.h"

namespace emu::applications::zxspectrum_48k {

u8 CpuIo::border_color()
{
    return m_out_port0xfe & s_border_color_mask;
}

u8 CpuIo::keyboard_input(u16 port)
{
    if (port == 0xfdfe) {
        return m_keyboard_0xfdfe;
    } else if (port == 0xfefe) {
        return m_keyboard_0xfefe;
    } else if (port == 0xfbfe) {
        return m_keyboard_0xfbfe;
    } else if (port == 0xf7fe) {
        return m_keyboard_0xf7fe;
    } else if (port == 0xeffe) {
        return m_keyboard_0xeffe;
    } else if (port == 0xdffe) {
        return m_keyboard_0xdffe;
    } else if (port == 0xbffe) {
        return m_keyboard_0xbffe;
    } else if (port == 0x7ffe) {
        return m_keyboard_0x7ffe;
    }

    return 0;
}
}
