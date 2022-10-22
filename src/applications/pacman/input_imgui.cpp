#include <algorithm>
#include <SDL.h>
#include "input_imgui.h"
#include "imgui_impl_sdl.h"
#include "interfaces/io_observer.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/misc/run_status.h"

namespace emu::applications::pacman {

    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;
    using emu::misc::RunStatus;
    using emu::misc::RunStatus::NOT_RUNNING;
    using emu::misc::RunStatus::RUNNING;
    using emu::misc::RunStatus::PAUSED;
    using emu::misc::RunStatus::STEPPING;

    void InputImgui::add_io_observer(IoObserver &observer) {
        m_io_observers.push_back(&observer);
    }

    void InputImgui::remove_io_observer(IoObserver *observer) {
        m_io_observers.erase(
                std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
                m_io_observers.end()
        );
    }

    void InputImgui::notify_io_observers(IoRequest request) {
        for (IoObserver *observer: m_io_observers) {
            observer->io_changed(request);
        }
    }

    void InputImgui::read(RunStatus &run_status, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io) {
        SDL_Event read_input_event;

        while (SDL_PollEvent(&read_input_event) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&read_input_event);

            ImGuiIO &io = ImGui::GetIO();

            if (!io.WantCaptureKeyboard) {
                switch (read_input_event.type) {
                    case SDL_QUIT:
                        run_status = RunStatus::NOT_RUNNING;
                        break;
                    case SDL_KEYUP:
                        switch (read_input_event.key.keysym.scancode) {
                            case insert_coin_p1:
                                memory_mapped_io->in0_read(5, true);
                                break;
                            case insert_coin_p2:
                                memory_mapped_io->in0_read(6, true);
                                break;
                            case p1_start:
                                memory_mapped_io->in1_read(5, true);
                                break;
                            case p1_up:
                                memory_mapped_io->in0_read(0, true);
                                break;
                            case p1_down:
                                memory_mapped_io->in0_read(3, true);
                                break;
                            case p1_left:
                                memory_mapped_io->in0_read(1, true);
                                break;
                            case p1_right:
                                memory_mapped_io->in0_read(2, true);
                                break;
                            case p2_start:
                                memory_mapped_io->in1_read(6, true);
                                break;
                            case p2_up:
                                memory_mapped_io->in1_read(0, true);
                                break;
                            case p2_down:
                                memory_mapped_io->in1_read(3, true);
                                break;
                            case p2_left:
                                memory_mapped_io->in1_read(1, true);
                                break;
                            case p2_right:
                                memory_mapped_io->in1_read(2, true);
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_KEYDOWN:
                        switch (read_input_event.key.keysym.scancode) {
                            case tile_debug:
                                notify_io_observers(IoRequest::TOGGLE_TILE_DEBUG);
                                break;
                            case sprite_debug:
                                notify_io_observers(IoRequest::TOGGLE_SPRITE_DEBUG);
                                break;
                            case mute:
                                notify_io_observers(IoRequest::TOGGLE_MUTE);
                                break;
                            case pause:
                                if (run_status == RunStatus::PAUSED) {
                                    run_status = RunStatus::RUNNING;
                                } else if (run_status == RunStatus::RUNNING) {
                                    run_status = RunStatus::PAUSED;
                                }
                                break;
                            case insert_coin_p1:
                                memory_mapped_io->in0_read(5, false);
                                break;
                            case insert_coin_p2:
                                memory_mapped_io->in0_read(6, false);
                                break;
                            case p1_start:
                                memory_mapped_io->in1_read(5, false);
                                break;
                            case p1_up:
                                memory_mapped_io->in0_read(0, false);
                                break;
                            case p1_down:
                                memory_mapped_io->in0_read(3, false);
                                break;
                            case p1_left:
                                memory_mapped_io->in0_read(1, false);
                                break;
                            case p1_right:
                                memory_mapped_io->in0_read(2, false);
                                break;
                            case p2_start:
                                memory_mapped_io->in1_read(6, false);
                                break;
                            case p2_up:
                                memory_mapped_io->in1_read(0, false);
                                break;
                            case p2_down:
                                memory_mapped_io->in1_read(3, false);
                                break;
                            case p2_left:
                                memory_mapped_io->in1_read(1, false);
                                break;
                            case p2_right:
                                memory_mapped_io->in1_read(2, false);
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

    void InputImgui::read_debug_only(RunStatus &run_status) {
        SDL_Event read_input_event;

        while (SDL_PollEvent(&read_input_event) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&read_input_event);

            ImGuiIO &io = ImGui::GetIO();

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
