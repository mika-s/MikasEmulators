#include "input_imgui.h"
#include "gui_io.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/key_observer.h"
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
                case s_credit:
                    memory_mapped_io->in0_read(7, true);
                    break;
                case s_insert_coin_p1:
                    memory_mapped_io->in0_read(5, true);
                    break;
                case s_insert_coin_p2:
                    memory_mapped_io->in0_read(6, true);
                    break;
                case s_p1_start:
                    memory_mapped_io->in1_read(5, true);
                    break;
                case s_p1_up:
                    memory_mapped_io->in0_read(0, true);
                    break;
                case s_p1_down:
                    memory_mapped_io->in0_read(3, true);
                    break;
                case s_p1_left:
                    memory_mapped_io->in0_read(1, true);
                    break;
                case s_p1_right:
                    memory_mapped_io->in0_read(2, true);
                    break;
                case s_p2_start:
                    memory_mapped_io->in1_read(6, true);
                    break;
                case s_p2_up:
                    memory_mapped_io->in1_read(0, true);
                    break;
                case s_p2_down:
                    memory_mapped_io->in1_read(3, true);
                    break;
                case s_p2_left:
                    memory_mapped_io->in1_read(1, true);
                    break;
                case s_p2_right:
                    memory_mapped_io->in1_read(2, true);
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
                case s_credit:
                    memory_mapped_io->in0_read(7, false);
                    break;
                case s_insert_coin_p1:
                    memory_mapped_io->in0_read(5, false);
                    break;
                case s_insert_coin_p2:
                    memory_mapped_io->in0_read(6, false);
                    break;
                case s_p1_start:
                    memory_mapped_io->in1_read(5, false);
                    break;
                case s_p1_up:
                    memory_mapped_io->in0_read(0, false);
                    break;
                case s_p1_down:
                    memory_mapped_io->in0_read(3, false);
                    break;
                case s_p1_left:
                    memory_mapped_io->in0_read(1, false);
                    break;
                case s_p1_right:
                    memory_mapped_io->in0_read(2, false);
                    break;
                case s_p2_start:
                    memory_mapped_io->in1_read(6, false);
                    break;
                case s_p2_up:
                    memory_mapped_io->in1_read(0, false);
                    break;
                case s_p2_down:
                    memory_mapped_io->in1_read(3, false);
                    break;
                case s_p2_left:
                    memory_mapped_io->in1_read(1, false);
                    break;
                case s_p2_right:
                    memory_mapped_io->in1_read(2, false);
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
