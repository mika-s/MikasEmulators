#include "input_imgui.h"
#include "cpu_io.h"
#include "crosscutting/util/byte_util.h"
#include "gui_io.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/key_observer.h"
#include "key_request.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

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
                case SDL_SCANCODE_LSHIFT:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
                    break;
                case SDL_SCANCODE_Z:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_Z_bit);
                    break;
                case SDL_SCANCODE_X:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_X_bit);
                    break;
                case SDL_SCANCODE_C:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_C_bit);
                    break;
                case SDL_SCANCODE_V:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_V_bit);
                    break;
                case SDL_SCANCODE_A:
                    set_bit(cpu_io.m_keyboard_0xfdfe, s_A_bit);
                    break;
                case SDL_SCANCODE_S:
                    set_bit(cpu_io.m_keyboard_0xfdfe, s_S_bit);
                    break;
                case SDL_SCANCODE_D:
                    set_bit(cpu_io.m_keyboard_0xfdfe, s_D_bit);
                    break;
                case SDL_SCANCODE_F:
                    set_bit(cpu_io.m_keyboard_0xfdfe, s_F_bit);
                    break;
                case SDL_SCANCODE_G:
                    set_bit(cpu_io.m_keyboard_0xfdfe, s_G_bit);
                    break;
                case SDL_SCANCODE_Q:
                    set_bit(cpu_io.m_keyboard_0xfbfe, s_Q_bit);
                    break;
                case SDL_SCANCODE_W:
                    set_bit(cpu_io.m_keyboard_0xfbfe, s_W_bit);
                    break;
                case SDL_SCANCODE_E:
                    set_bit(cpu_io.m_keyboard_0xfbfe, s_E_bit);
                    break;
                case SDL_SCANCODE_R:
                    set_bit(cpu_io.m_keyboard_0xfbfe, s_R_bit);
                    break;
                case SDL_SCANCODE_T:
                    set_bit(cpu_io.m_keyboard_0xfbfe, s_T_bit);
                    break;
                case SDL_SCANCODE_1:
                    set_bit(cpu_io.m_keyboard_0xf7fe, s_1_bit);
                    break;
                case SDL_SCANCODE_2:
                    set_bit(cpu_io.m_keyboard_0xf7fe, s_2_bit);
                    break;
                case SDL_SCANCODE_3:
                    set_bit(cpu_io.m_keyboard_0xf7fe, s_3_bit);
                    break;
                case SDL_SCANCODE_4:
                    set_bit(cpu_io.m_keyboard_0xf7fe, s_4_bit);
                    break;
                case SDL_SCANCODE_5:
                    set_bit(cpu_io.m_keyboard_0xf7fe, s_5_bit);
                    break;
                case SDL_SCANCODE_0:
                    set_bit(cpu_io.m_keyboard_0xeffe, s_0_bit);
                    break;
                case SDL_SCANCODE_9:
                    set_bit(cpu_io.m_keyboard_0xeffe, s_9_bit);
                    break;
                case SDL_SCANCODE_8:
                    set_bit(cpu_io.m_keyboard_0xeffe, s_8_bit);
                    break;
                case SDL_SCANCODE_7:
                    set_bit(cpu_io.m_keyboard_0xeffe, s_7_bit);
                    break;
                case SDL_SCANCODE_6:
                    set_bit(cpu_io.m_keyboard_0xeffe, s_6_bit);
                    break;
                case SDL_SCANCODE_P:
                    set_bit(cpu_io.m_keyboard_0xdffe, s_P_bit);
                    break;
                case SDL_SCANCODE_O:
                    set_bit(cpu_io.m_keyboard_0xdffe, s_O_bit);
                    break;
                case SDL_SCANCODE_I:
                    set_bit(cpu_io.m_keyboard_0xdffe, s_I_bit);
                    break;
                case SDL_SCANCODE_U:
                    set_bit(cpu_io.m_keyboard_0xdffe, s_U_bit);
                    break;
                case SDL_SCANCODE_Y:
                    set_bit(cpu_io.m_keyboard_0xdffe, s_Y_bit);
                    break;
                case SDL_SCANCODE_RETURN:
                    set_bit(cpu_io.m_keyboard_0xbffe, s_ENTER_bit);
                    break;
                case SDL_SCANCODE_L:
                    set_bit(cpu_io.m_keyboard_0xbffe, s_L_bit);
                    break;
                case SDL_SCANCODE_K:
                    set_bit(cpu_io.m_keyboard_0xbffe, s_K_bit);
                    break;
                case SDL_SCANCODE_J:
                    set_bit(cpu_io.m_keyboard_0xbffe, s_J_bit);
                    break;
                case SDL_SCANCODE_H:
                    set_bit(cpu_io.m_keyboard_0xbffe, s_H_bit);
                    break;
                case SDL_SCANCODE_SPACE:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SPACE_bit);
                    break;
                case SDL_SCANCODE_RSHIFT:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    break;
                case SDL_SCANCODE_M:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_M_bit);
                    break;
                case SDL_SCANCODE_N:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_N_bit);
                    break;
                case SDL_SCANCODE_B:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_B_bit);
                    break;
                    /* Non-physical: */
                case SDL_SCANCODE_BACKSPACE:
                    set_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
                    set_bit(cpu_io.m_keyboard_0xeffe, s_0_bit);
                    break;
                case SDL_SCANCODE_COMMA:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_N_bit);
                    break;
                case SDL_SCANCODE_PERIOD:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_M_bit);
                    break;
                case SDL_SCANCODE_SLASH:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    set_bit(cpu_io.m_keyboard_0xfefe, s_V_bit);
                    break;
                case SDL_SCANCODE_TAB:
                    set_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    set_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case SDL_SCANCODE_LSHIFT:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
                    break;
                case SDL_SCANCODE_Z:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_Z_bit);
                    break;
                case SDL_SCANCODE_X:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_X_bit);
                    break;
                case SDL_SCANCODE_C:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_C_bit);
                    break;
                case SDL_SCANCODE_V:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_V_bit);
                    break;
                case SDL_SCANCODE_A:
                    unset_bit(cpu_io.m_keyboard_0xfdfe, s_A_bit);
                    break;
                case SDL_SCANCODE_S:
                    unset_bit(cpu_io.m_keyboard_0xfdfe, s_S_bit);
                    break;
                case SDL_SCANCODE_D:
                    unset_bit(cpu_io.m_keyboard_0xfdfe, s_D_bit);
                    break;
                case SDL_SCANCODE_F:
                    unset_bit(cpu_io.m_keyboard_0xfdfe, s_F_bit);
                    break;
                case SDL_SCANCODE_G:
                    unset_bit(cpu_io.m_keyboard_0xfdfe, s_G_bit);
                    break;
                case SDL_SCANCODE_Q:
                    unset_bit(cpu_io.m_keyboard_0xfbfe, s_Q_bit);
                    break;
                case SDL_SCANCODE_W:
                    unset_bit(cpu_io.m_keyboard_0xfbfe, s_W_bit);
                    break;
                case SDL_SCANCODE_E:
                    unset_bit(cpu_io.m_keyboard_0xfbfe, s_E_bit);
                    break;
                case SDL_SCANCODE_R:
                    unset_bit(cpu_io.m_keyboard_0xfbfe, s_R_bit);
                    break;
                case SDL_SCANCODE_T:
                    unset_bit(cpu_io.m_keyboard_0xfbfe, s_T_bit);
                    break;
                case SDL_SCANCODE_1:
                    unset_bit(cpu_io.m_keyboard_0xf7fe, s_1_bit);
                    break;
                case SDL_SCANCODE_2:
                    unset_bit(cpu_io.m_keyboard_0xf7fe, s_2_bit);
                    break;
                case SDL_SCANCODE_3:
                    unset_bit(cpu_io.m_keyboard_0xf7fe, s_3_bit);
                    break;
                case SDL_SCANCODE_4:
                    unset_bit(cpu_io.m_keyboard_0xf7fe, s_4_bit);
                    break;
                case SDL_SCANCODE_5:
                    unset_bit(cpu_io.m_keyboard_0xf7fe, s_5_bit);
                    break;
                case SDL_SCANCODE_0:
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_0_bit);
                    break;
                case SDL_SCANCODE_9:
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_9_bit);
                    break;
                case SDL_SCANCODE_8:
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_8_bit);
                    break;
                case SDL_SCANCODE_7:
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_7_bit);
                    break;
                case SDL_SCANCODE_6:
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_6_bit);
                    break;
                case SDL_SCANCODE_P:
                    unset_bit(cpu_io.m_keyboard_0xdffe, s_P_bit);
                    break;
                case SDL_SCANCODE_O:
                    unset_bit(cpu_io.m_keyboard_0xdffe, s_O_bit);
                    break;
                case SDL_SCANCODE_I:
                    unset_bit(cpu_io.m_keyboard_0xdffe, s_I_bit);
                    break;
                case SDL_SCANCODE_U:
                    unset_bit(cpu_io.m_keyboard_0xdffe, s_U_bit);
                    break;
                case SDL_SCANCODE_Y:
                    unset_bit(cpu_io.m_keyboard_0xdffe, s_Y_bit);
                    break;
                case SDL_SCANCODE_RETURN:
                    unset_bit(cpu_io.m_keyboard_0xbffe, s_ENTER_bit);
                    break;
                case SDL_SCANCODE_L:
                    unset_bit(cpu_io.m_keyboard_0xbffe, s_L_bit);
                    break;
                case SDL_SCANCODE_K:
                    unset_bit(cpu_io.m_keyboard_0xbffe, s_K_bit);
                    break;
                case SDL_SCANCODE_J:
                    unset_bit(cpu_io.m_keyboard_0xbffe, s_J_bit);
                    break;
                case SDL_SCANCODE_H:
                    unset_bit(cpu_io.m_keyboard_0xbffe, s_H_bit);
                    break;
                case SDL_SCANCODE_SPACE:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SPACE_bit);
                    break;
                case SDL_SCANCODE_RSHIFT:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    break;
                case SDL_SCANCODE_M:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_M_bit);
                    break;
                case SDL_SCANCODE_N:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_N_bit);
                    break;
                case SDL_SCANCODE_B:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_B_bit);
                    break;
                    /* Non-physical: */
                case SDL_SCANCODE_BACKSPACE:
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
                    unset_bit(cpu_io.m_keyboard_0xeffe, s_0_bit);
                    break;
                case SDL_SCANCODE_COMMA:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_N_bit);
                    break;
                case SDL_SCANCODE_PERIOD:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_M_bit);
                    break;
                case SDL_SCANCODE_SLASH:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_V_bit);
                    break;
                case SDL_SCANCODE_TAB:
                    unset_bit(cpu_io.m_keyboard_0x7ffe, s_SYM_bit);
                    unset_bit(cpu_io.m_keyboard_0xfefe, s_SHIFT_bit);
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
