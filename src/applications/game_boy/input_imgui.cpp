#include "input_imgui.h"
#include "gui_io.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/interrupt_observer.h"
#include "interfaces/key_observer.h"
#include "interrupts.h"
#include "key_request.h"
#include "memory_mapped_io_for_game_boy.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::game_boy {

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

void InputImgui::notify_io_observers(IoRequest request)
{
    for (KeyObserver* observer : m_io_observers) {
        observer->key_pressed(request);
    }
}

void InputImgui::add_interrupt_observer(InterruptObserver& observer)
{
    m_interrupt_observers.push_back(&observer);
}

void InputImgui::remove_interrupt_observer(InterruptObserver* observer)
{
    m_interrupt_observers.erase(
        std::remove(m_interrupt_observers.begin(), m_interrupt_observers.end(), observer),
        m_interrupt_observers.end());
}

void InputImgui::read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io)
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
                case s_p1_start:
                    m_is_pressing_start = false;
                    memory_mapped_io->p1_button_keys(s_bit_number_start, true);
                    break;
                case s_p1_select:
                    m_is_pressing_select = false;
                    memory_mapped_io->p1_button_keys(s_bit_number_select, true);
                    break;
                case s_up:
                    m_is_pressing_up = false;
                    memory_mapped_io->p1_direction_keys(s_bit_number_up, true);
                    break;
                case s_down:
                    m_is_pressing_down = false;
                    memory_mapped_io->p1_direction_keys(s_bit_number_down, true);
                    break;
                case s_left:
                    m_is_pressing_left = false;
                    memory_mapped_io->p1_direction_keys(s_bit_number_left, true);
                    break;
                case s_right:
                    m_is_pressing_right = false;
                    memory_mapped_io->p1_direction_keys(s_bit_number_right, true);
                    break;
                case s_a:
                    m_is_pressing_a = false;
                    memory_mapped_io->p1_button_keys(s_bit_number_a, true);
                    break;
                case s_b:
                    m_is_pressing_b = false;
                    memory_mapped_io->p1_button_keys(s_bit_number_b, true);
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_tile_debug:
                    notify_io_observers(TOGGLE_TILE_DEBUG);
                    break;
                case s_sprite_debug:
                    notify_io_observers(TOGGLE_SPRITE_DEBUG);
                    break;
                case s_mute:
                    notify_io_observers(TOGGLE_MUTE);
                    break;
                case s_pause:
                    gui_io.m_is_toggling_pause = true;
                    break;
                case s_p1_start:
                    if (!m_is_pressing_start) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_start = true;
                    memory_mapped_io->p1_button_keys(s_bit_number_start, false);
                    break;
                case s_p1_select:
                    if (!m_is_pressing_select) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_select = true;
                    memory_mapped_io->p1_button_keys(s_bit_number_select, false);
                    break;
                case s_up:
                    if (!m_is_pressing_up) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_up = true;
                    memory_mapped_io->p1_direction_keys(s_bit_number_up, false);
                    break;
                case s_down:
                    if (!m_is_pressing_down) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_down = true;
                    memory_mapped_io->p1_direction_keys(s_bit_number_down, false);
                    break;
                case s_left:
                    if (!m_is_pressing_left) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_left = true;
                    memory_mapped_io->p1_direction_keys(s_bit_number_left, false);
                    break;
                case s_right:
                    if (!m_is_pressing_right) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_right = true;
                    memory_mapped_io->p1_direction_keys(s_bit_number_right, false);
                    break;
                case s_a:
                    if (!m_is_pressing_a) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_a = true;
                    memory_mapped_io->p1_button_keys(s_bit_number_a, false);
                    break;
                case s_b:
                    if (!m_is_pressing_b) {
                        notify_interrupt_observers(JOYPAD);
                    }
                    m_is_pressing_b = true;
                    memory_mapped_io->p1_button_keys(s_bit_number_b, false);
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

void InputImgui::notify_interrupt_observers(Interrupts interrupt)
{
    for (InterruptObserver* observer : m_interrupt_observers) {
        observer->interrupt(interrupt);
    }
}

}
