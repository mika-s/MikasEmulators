#include "cpu_io.h"
#include <iterator>

namespace emu::applications::zxspectrum_48k {

u8 CpuIo::border_color()
{
    return m_out_port0xfe & s_border_color_mask;
}

void CpuIo::add_key_canceler(std::function<void()> const& cancel_func)
{
    m_cancel_last_keypress.emplace_back(s_count_before_key_cancel, cancel_func);
}

u8 CpuIo::keyboard_input(u16 port)
{
    const u8 value = m_keyboard.at(port);

    reset_key_presses();

    return value;
}

u8 CpuIo::keyboard_input(u16 port) const
{
    return m_keyboard.at(port);
}

void CpuIo::reset_key_presses()
{
    auto to_cancel = std::begin(m_cancel_last_keypress);

    while (to_cancel != std::end(m_cancel_last_keypress)) {
        if (to_cancel->first == 0) {
            to_cancel->second();
            to_cancel = m_cancel_last_keypress.erase(to_cancel);
        } else {
            to_cancel->first--;
            ++to_cancel;
        }
    }
}
}
