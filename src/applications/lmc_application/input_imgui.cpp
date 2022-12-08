#include "input_imgui.h"
#include "crosscutting/util/byte_util.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/input.h"
#include "interfaces/io_observer.h"
#include "io_request.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::lmc {

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

void InputImgui::read(RunStatus& run_status)
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
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (read_input_event.key.keysym.scancode) {
                case pause:
                    if (run_status == RunStatus::PAUSED) {
                        run_status = RunStatus::RUNNING;
                    } else if (run_status == RunStatus::RUNNING) {
                        run_status = RunStatus::PAUSED;
                    }
                    break;
                default:
                    break;
                }
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
                case pause:
                    if (run_status == PAUSED) {
                        run_status = STEPPING;
                    } else if (run_status == STEPPING) {
                        run_status = PAUSED;
                    }
                    break;
                case step_instruction:
                    notify_io_observers(STEP_INSTRUCTION);
                    break;
                case step_cycle:
                    notify_io_observers(STEP_CYCLE);
                    break;
                case continue_running:
                    notify_io_observers(CONTINUE_EXECUTION);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}
}
