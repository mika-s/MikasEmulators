#pragma once

#include "crosscutting/typedefs.h"
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications::zxspectrum_48k {

class CpuIo {
public:
    u8 m_out_port0xfe { 0x00 };

    std::unordered_map<u16, u8> m_keyboard = {
        { 0xfefe, 0xff }, // SHIFT, Z, X, C, V
        { 0xfdfe, 0xff }, // A, S, D, F, G
        { 0xfbfe, 0xff }, // Q, W, E, R, T
        { 0xf7fe, 0xff }, // 1, 2, 3, 4, 5
        { 0xeffe, 0xff }, // 0, 9, 8, 7, 6
        { 0xdffe, 0xff }, // P, O, I, U, Y
        { 0xbffe, 0xff }, // ENTER, L, K, J, H
        { 0x7ffe, 0xff }  // SPACE, SYM, M, N, B
    };

    std::vector<std::pair<unsigned int, std::function<void()>>> m_cancel_last_keypress;

    u8 border_color();

    void add_key_canceler(std::function<void()> const& cancel_func);

    u8 keyboard_input(u16 port);

    u8 keyboard_input(u16 port) const;

private:
    static constexpr u8 s_border_color_mask = 0b00000111;

    static constexpr unsigned int s_count_before_key_cancel = 10;

    void reset_key_presses();
};
}
