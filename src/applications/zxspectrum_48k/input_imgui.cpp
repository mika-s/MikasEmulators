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

void InputImgui::handle_text(CpuIo& cpu_io, std::string text)
{
    switch (text[0]) {
    case 'z':
    case 'Z':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit); });
        break;
    case 'x':
    case 'X':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_X_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_X_bit); });
        break;
    case 'c':
    case 'C':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_C_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_C_bit); });
        break;
    case 'v':
    case 'V':
        unset_bit(cpu_io.m_keyboard[0xfefe], s_V_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfefe], s_V_bit); });
        break;
    case 'a':
    case 'A':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_A_bit); });
        break;
    case 's':
    case 'S':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_S_bit); });
        break;
    case 'd':
    case 'D':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_D_bit); });
        break;
    case 'f':
    case 'F':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_F_bit); });
        break;
    case 'g':
    case 'G':
        unset_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfdfe], s_G_bit); });
        break;
    case 'q':
    case 'Q':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_Q_bit); });
        break;
    case 'w':
    case 'W':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_W_bit); });
        break;
    case 'e':
    case 'E':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_E_bit); });
        break;
    case 'r':
    case 'R':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_R_bit); });
        break;
    case 't':
    case 'T':
        unset_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xfbfe], s_T_bit); });
        break;
    case 'p':
    case 'P':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_P_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_P_bit); });
        break;
    case 'o':
    case 'O':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_O_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_O_bit); });
        break;
    case 'i':
    case 'I':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_I_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_I_bit); });
        break;
    case 'u':
    case 'U':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_U_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_U_bit); });
        break;
    case 'y':
    case 'Y':
        unset_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xdffe], s_Y_bit); });
        break;
    case 'l':
    case 'L':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_L_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_L_bit); });
        break;
    case 'k':
    case 'K':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_K_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_K_bit); });
        break;
    case 'j':
    case 'J':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_J_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_J_bit); });
        break;
    case 'h':
    case 'H':
        unset_bit(cpu_io.m_keyboard[0xbffe], s_H_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0xbffe], s_H_bit); });
        break;
    case ' ':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SPACE_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SPACE_bit); });
        break;
    case 'm':
    case 'M':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit); });
        break;
    case 'n':
    case 'N':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit); });
        break;
    case 'b':
    case 'B':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_B_bit); });
        break;
    case ',':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0x7ffe], s_N_bit); });
        break;
    case '.':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0x7ffe], s_M_bit); });
        break;
    case ':':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xfefe], s_Z_bit); });
        break;
    case ';':
        unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
        unset_bit(cpu_io.m_keyboard[0xdffe], s_O_bit);
        m_cancel_last_keypress.emplace_back([&] { set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit); set_bit(cpu_io.m_keyboard[0xdffe], s_O_bit); });
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
            case SDL_KEYUP:
                switch (read_input_event.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                    set_bit(cpu_io.m_keyboard[0xbffe], s_ENTER_bit);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    set_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
                    break;
                case SDL_SCANCODE_LSHIFT:
                    set_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    break;
                case SDL_SCANCODE_RSHIFT:
                    set_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
                    break;
                case SDL_SCANCODE_1:
                    set_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit);
                    break;
                case SDL_SCANCODE_2:
                    set_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit);
                    break;
                case SDL_SCANCODE_3:
                    set_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit);
                    break;
                case SDL_SCANCODE_4:
                    set_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit);
                    break;
                case SDL_SCANCODE_5:
                    set_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit);
                    break;
                case SDL_SCANCODE_0:
                    set_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
                    break;
                case SDL_SCANCODE_9:
                    set_bit(cpu_io.m_keyboard[0xeffe], s_9_bit);
                    break;
                case SDL_SCANCODE_8:
                    set_bit(cpu_io.m_keyboard[0xeffe], s_8_bit);
                    break;
                case SDL_SCANCODE_7:
                    set_bit(cpu_io.m_keyboard[0xeffe], s_7_bit);
                    break;
                case SDL_SCANCODE_6:
                    set_bit(cpu_io.m_keyboard[0xeffe], s_6_bit);
                    break;
                default: {
                    // Reset text input
                    for (auto& to_cancel : m_cancel_last_keypress) {
                        to_cancel();
                    }
                } break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case SDL_SCANCODE_RETURN:
                    unset_bit(cpu_io.m_keyboard[0xbffe], s_ENTER_bit);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
                    break;
                case SDL_SCANCODE_LSHIFT:
                    unset_bit(cpu_io.m_keyboard[0xfefe], s_SHIFT_bit);
                    break;
                case SDL_SCANCODE_RSHIFT:
                    unset_bit(cpu_io.m_keyboard[0x7ffe], s_SYM_bit);
                    break;
                case SDL_SCANCODE_1:
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_1_bit);
                    break;
                case SDL_SCANCODE_2:
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_2_bit);
                    break;
                case SDL_SCANCODE_3:
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_3_bit);
                    break;
                case SDL_SCANCODE_4:
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_4_bit);
                    break;
                case SDL_SCANCODE_5:
                    unset_bit(cpu_io.m_keyboard[0xf7fe], s_5_bit);
                    break;
                case SDL_SCANCODE_0:
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_0_bit);
                    break;
                case SDL_SCANCODE_9:
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_9_bit);
                    break;
                case SDL_SCANCODE_8:
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_8_bit);
                    break;
                case SDL_SCANCODE_7:
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_7_bit);
                    break;
                case SDL_SCANCODE_6:
                    unset_bit(cpu_io.m_keyboard[0xeffe], s_6_bit);
                    break;
                default:
                    break;
                }
                break;
            case SDL_TEXTINPUT:
                handle_text(cpu_io, std::string(read_input_event.text.text));
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
