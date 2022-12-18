#include "input_imgui.h"
#include "crosscutting/util/byte_util.h"
#include "gui_io.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "space_invaders/cpu_io.h"
#include "space_invaders/interfaces/key_observer.h"
#include "space_invaders/key_request.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::space_invaders {

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

/*
 Ports:
    Read 1
    BIT	0	coin (0 when active)
        1	P2 start button
        2	P1 start button
        3	?
        4	P1 shoot button
        5	P1 joystick left
        6	P1 joystick right
        7	?

    Read 2
    BIT	0,1	dipswitch number of lives (0:3,1:4,2:5,3:6)
        2	s_tilt 'button'
        3	dipswitch bonus life at 1:1000,0:1500
        4	P2 shoot button
        5	P2 joystick left
        6	P2 joystick right
        7	dipswitch coin info 1:off,0:on
*/
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
                case s_insert_coin:
                    unset_bit(cpu_io.m_in_port1, 0);
                    break;
                case s_tilt:
                    unset_bit(cpu_io.m_in_port2, 2);
                    break;
                case s_p1_start:
                    unset_bit(cpu_io.m_in_port1, 2);
                    break;
                case s_p1_shoot:
                    unset_bit(cpu_io.m_in_port1, 4);
                    break;
                case s_p1_left:
                    unset_bit(cpu_io.m_in_port1, 5);
                    break;
                case s_p1_right:
                    unset_bit(cpu_io.m_in_port1, 6);
                    break;
                case s_p2_start:
                    unset_bit(cpu_io.m_in_port1, 1);
                    break;
                case s_p2_shoot:
                    unset_bit(cpu_io.m_in_port2, 4);
                    break;
                case s_p2_left:
                    unset_bit(cpu_io.m_in_port2, 5);
                    break;
                case s_p2_right:
                    unset_bit(cpu_io.m_in_port2, 6);
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_mute:
                    notify_io_observers(TOGGLE_MUTE);
                    break;
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case s_insert_coin:
                    set_bit(cpu_io.m_in_port1, 0);
                    break;
                case s_tilt:
                    set_bit(cpu_io.m_in_port2, 2);
                    break;
                case s_p1_start:
                    set_bit(cpu_io.m_in_port1, 2);
                    break;
                case s_p1_shoot:
                    set_bit(cpu_io.m_in_port1, 4);
                    break;
                case s_p1_left:
                    set_bit(cpu_io.m_in_port1, 5);
                    break;
                case s_p1_right:
                    set_bit(cpu_io.m_in_port1, 6);
                    break;
                case s_p2_start:
                    set_bit(cpu_io.m_in_port1, 1);
                    break;
                case s_p2_shoot:
                    set_bit(cpu_io.m_in_port2, 4);
                    break;
                case s_p2_left:
                    set_bit(cpu_io.m_in_port2, 5);
                    break;
                case s_p2_right:
                    set_bit(cpu_io.m_in_port2, 6);
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

void InputImgui::read_debug_only(GuiIo& gui_io)
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
