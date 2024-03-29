#include "input_imgui.h"
#include "cpu_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "gui_io.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/key_observer.h"
#include "key_request.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

namespace emu::applications::zxspectrum_48k {

using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;

void InputImgui::add_io_observer(KeyObserver& observer)
{
    m_io_observers.push_back(&observer);
}

void InputImgui::remove_io_observer(KeyObserver* observer)
{
    m_io_observers.erase(
        std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
        m_io_observers.end());
}

void InputImgui::notify_io_observers(KeyRequest request)
{
    for (KeyObserver* observer : m_io_observers) {
        observer->key_pressed(request);
    }
}

void InputImgui::cancel_shift(CpuIo& cpu_io)
{
    set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
}

void InputImgui::handle_text(CpuIo& cpu_io, std::string text, bool is_shift_pressed)
{
    if (is_shift_pressed) {
        cancel_shift(cpu_io);
    }

    switch (text[0]) {
    case 'z':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit); });
        break;
    case 'Z':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'x':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_X_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_X_bit); });
        break;
    case 'X':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_X_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_X_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'c':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_C_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_C_bit); });
        break;
    case 'C':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_C_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_C_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'v':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_V_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_V_bit); });
        break;
    case 'V':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_V_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_V_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'a':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit); });
        break;
    case 'A':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 's':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit); });
        break;
    case 'S':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'd':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit); });
        break;
    case 'D':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'f':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit); });
        break;
    case 'F':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'g':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit); });
        break;
    case 'G':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'q':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit); });
        break;
    case 'Q':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'w':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit); });
        break;
    case 'W':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'e':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit); });
        break;
    case 'E':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'r':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit); });
        break;
    case 'R':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 't':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit); });
        break;
    case 'T':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case '1':
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit); });
        break;
    case '2':
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit); });
        break;
    case '3':
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit); });
        break;
    case '4':
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit); });
        break;
    case '5':
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit); });
        break;
    case '0':
        unset_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xeffe], s_0_bit); });
        break;
    case '9':
        unset_bit(cpu_io.m_keyboard[0xeffe], s_9_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xeffe], s_9_bit); });
        break;
    case '8':
        unset_bit(cpu_io.m_keyboard[0xeffe], s_8_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xeffe], s_8_bit); });
        break;
    case '7':
        unset_bit(cpu_io.m_keyboard[0xeffe], s_7_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xeffe], s_7_bit); });
        break;
    case '6':
        unset_bit(cpu_io.m_keyboard[0xeffe], s_6_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xeffe], s_6_bit); });
        break;
    case 'p':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_P_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_P_bit); });
        break;
    case 'P':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_P_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_P_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'o':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_O_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_O_bit); });
        break;
    case 'O':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_O_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_O_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'i':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_I_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_I_bit); });
        break;
    case 'I':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_I_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_I_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'u':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_U_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_U_bit); });
        break;
    case 'U':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_U_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_U_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'y':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit); });
        break;
    case 'Y':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'l':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_L_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_L_bit); });
        break;
    case 'L':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_L_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_L_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'k':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_K_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_K_bit); });
        break;
    case 'K':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_K_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_K_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'j':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_J_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_J_bit); });
        break;
    case 'J':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_J_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_J_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'h':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_H_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_H_bit); });
        break;
    case 'H':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_H_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_H_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case ' ':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SPACE_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SPACE_bit); });
        break;
    case 'm':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit); });
        break;
    case 'M':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'n':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit); });
        break;
    case 'N':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case 'b':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit); });
        break;
    case 'B':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
        break;
    case ',':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit); });
        break;
    case '.':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit); });
        break;
    case ':':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit); });
        break;
    case ';':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xdffe], s_O_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xdffe], s_O_bit); });
        break;
    case '"':
        std::cout << "Pressed \"\n";
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xdffe], s_P_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xdffe], s_P_bit); });
        break;
    case '!':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit); });
        break;
    case '@':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit); });
        break;
    case '#':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit); });
        break;
    case '$':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit); });
        break;
    case '%':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit); });
        break;
    case '&':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xeffe], s_6_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_6_bit); });
        break;
    case '\'':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xeffe], s_7_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_7_bit); });
        break;
    case '(':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xeffe], s_8_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_8_bit); });
        break;
    case ')':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xeffe], s_9_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_9_bit); });
        break;
    case '_':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_0_bit); });
        break;
    case '-':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xbffe], s_J_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xbffe], s_J_bit); });
        break;
    case '+':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xbffe], s_K_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xbffe], s_K_bit); });
        break;
    case '/':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_V_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_V_bit); });
        break;
    case '*':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit); });
        break;
    case '=':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xbffe], s_L_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xbffe], s_L_bit); });
        break;
    case '<':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit); });
        break;
    case '>':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit); });
        break;
    case '?':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_C_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_C_bit); });
        break;
    case '^':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xbffe], s_H_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xbffe], s_H_bit); });
        break;
    case '~':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit); });
        break;
    case '{':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit); });
        break;
    case '}':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit); });
        break;
    case '[':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit); });
        break;
    case ']':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xdffe], s_U_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xdffe], s_U_bit); });
        break;
    case '|':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit); });
        break;
    case '\\':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit);
        cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit); });
        break;
    default:
        break;
    }
}

void InputImgui::read(CpuIo& cpu_io, GuiIo& gui_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&read_input_event);

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard) {
            switch (read_input_event.type) {
            case SDL_QUIT:
                gui_io.m_is_quitting = true;
                break;
            case SDL_TEXTINPUT:
                handle_text(cpu_io, std::string(read_input_event.text.text), io.KeyShift);
                break;
            case SDL_KEYUP:
                switch (read_input_event.key.keysym.scancode) {
                    //                case SDL_SCANCODE_LSHIFT: {
                    //                    // Reset text input
                    //                    for (auto& to_cancel : m_cancel_keypress_on_key_up) {
                    //                        to_cancel();
                    //                    }
                    //                    m_cancel_keypress_on_key_up.clear();
                    //                } break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case SDL_SCANCODE_RETURN:
                    unset_bit(cpu_io.m_keyboard[0xbffe], s_ENTER_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_ENTER_bit); });
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_0_bit); });
                    break;
                case SDL_SCANCODE_LSHIFT:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); });
                    break;
                case SDL_SCANCODE_LALT:
                    unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); });
                    break;
                case SDL_SCANCODE_LEFT:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit); });
                    break;
                case SDL_SCANCODE_DOWN:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_6_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_6_bit); });
                    break;
                case SDL_SCANCODE_UP:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_7_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_7_bit); });
                    break;
                case SDL_SCANCODE_RIGHT:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_8_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_8_bit); });
                    break;
                case SDL_SCANCODE_F1:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit); });
                    break;
                case SDL_SCANCODE_CAPSLOCK:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit); });
                    break;
                case SDL_SCANCODE_F3:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit); });
                    break;
                case SDL_SCANCODE_F4:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit); });
                    break;
                case SDL_SCANCODE_F9:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_9_bit);
                    cpu_io.add_key_canceler([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit); set_bit(cpu_io.m_keyboard[0xeffe], s_9_bit); });
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        } else {
            switch (read_input_event.type) {
            case SDL_QUIT:
                gui_io.m_is_quitting = true;
                break;
            default:
                break;
            }
        }
    }
}

void InputImgui::read_debug_only([[maybe_unused]] CpuIo& cpu_io, GuiIo& gui_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&read_input_event);

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard) {
            switch (read_input_event.type) {
            case SDL_QUIT:
                gui_io.m_is_quitting = true;
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case s_step_instruction:
                    gui_io.m_is_stepping_instruction = true;
                    break;
                case s_step_cycle:
                    gui_io.m_is_stepping_cycle = true;
                    break;
                case s_continue_running:
                    gui_io.m_is_continuing_execution = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        } else {
            switch (read_input_event.type) {
            case SDL_QUIT:
                gui_io.m_is_quitting = true;
                break;
            default:
                break;
            }
        }
    }
}
}
