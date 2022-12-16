#include "input_imgui.h"
#include "crosscutting/util/byte_util.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "pacman/interfaces/input.h"
#include "pacman/interfaces/io_observer.h"
#include "pacman/io_request.h"
#include "pacman/memory_mapped_io_for_pacman.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::pacman {

using emu::misc::RunStatus;
using emu::misc::RunStatus::NOT_RUNNING;
using emu::misc::RunStatus::PAUSED;
using emu::misc::RunStatus::RUNNING;
using emu::misc::RunStatus::STEPPING;
using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;

void InputImgui::add_io_observer(IoObserver& observer)
{
    m_io_observers.push_back(&observer);
}

void InputImgui::remove_io_observer(IoObserver* observer)
{
    m_io_observers.erase(
        std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
        m_io_observers.end());
}

void InputImgui::notify_io_observers(IoRequest request)
{
    for (IoObserver* observer : m_io_observers) {
        observer->io_changed(request);
    }
}

void InputImgui::read(RunStatus& run_status, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&read_input_event);

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard) {
            switch (read_input_event.type) {
            case SDL_QUIT:
                run_status = RunStatus::NOT_RUNNING;
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
                    notify_io_observers(IoRequest::TOGGLE_TILE_DEBUG);
                    break;
                case s_sprite_debug:
                    notify_io_observers(IoRequest::TOGGLE_SPRITE_DEBUG);
                    break;
                case s_mute:
                    notify_io_observers(IoRequest::TOGGLE_MUTE);
                    break;
                case s_pause:
                    if (run_status == RunStatus::PAUSED) {
                        run_status = RunStatus::RUNNING;
                    } else if (run_status == RunStatus::RUNNING) {
                        run_status = RunStatus::PAUSED;
                    }
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
                run_status = RunStatus::NOT_RUNNING;
                break;
            default:
                break;
            }
        }
    }
}

void InputImgui::read_debug_only(RunStatus& run_status)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&read_input_event);

        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureKeyboard) {
            switch (read_input_event.type) {
            case SDL_QUIT:
                run_status = NOT_RUNNING;
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case s_pause:
                    if (run_status == PAUSED) {
                        run_status = STEPPING;
                    } else if (run_status == STEPPING) {
                        run_status = PAUSED;
                    }
                    break;
                case s_step_instruction:
                    notify_io_observers(STEP_INSTRUCTION);
                    break;
                case s_step_cycle:
                    notify_io_observers(STEP_CYCLE);
                    break;
                case s_continue_running:
                    notify_io_observers(CONTINUE_EXECUTION);
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
                run_status = NOT_RUNNING;
                break;
            default:
                break;
            }
        }
    }
}
}
