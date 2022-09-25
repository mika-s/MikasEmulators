#include <algorithm>
#include <iostream>
#include <SDL.h>
#include "imgui_impl_sdl.h"
#include "input_sdl.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80::applications::pacman {

    using emu::util::byte::set_bit;
    using emu::util::byte::unset_bit;

    void InputSdl::add_io_observer(IoObserver &observer) {
        m_io_observers.push_back(&observer);

    }

    void InputSdl::remove_io_observer(IoObserver *observer) {
        m_io_observers.erase(
                std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
                m_io_observers.end()
        );
    }

    void InputSdl::notify_io_observers(IoRequest request) {
        for (IoObserver *observer: m_io_observers) {
            observer->io_changed(request);
        }
    }

    void InputSdl::read(RunStatus &run_status, std::shared_ptr<MemoryMappedIo> memory_mapped_io) {
        SDL_Event read_input_event;

        while (SDL_PollEvent(&read_input_event) != 0) {
            switch (read_input_event.type) {
                case SDL_QUIT:
                    run_status = RunStatus::NOT_RUNNING;
                    break;
                case SDL_KEYUP:
                    switch (read_input_event.key.keysym.scancode) {
                        case insert_coin:
                            memory_mapped_io->write_in0(5, true);
                            std::cout << "KEY UP: Insert coin\n";
                            break;
                        case p1_start:
                            memory_mapped_io->write_in1(5, true);
                            std::cout << "KEY UP: P1 start\n";
                            break;
                        case p1_up:
                            memory_mapped_io->write_in0(1, true);
                            std::cout << "KEY UP: P1 up\n";
                            break;
                        case p1_down:
                            memory_mapped_io->write_in0(4, true);
                            std::cout << "KEY UP: P1 down\n";
                            break;
                        case p1_left:
                            memory_mapped_io->write_in0(2, true);
                            std::cout << "KEY UP: P1 left\n";
                            break;
                        case p1_right:
                            memory_mapped_io->write_in0(3, true);
                            std::cout << "KEY UP: P1 right\n";
                            break;
                        case p2_start:
                            memory_mapped_io->write_in1(6, true);
                            std::cout << "KEY UP: P2 start\n";
                            break;
                        case p2_up:
                            memory_mapped_io->write_in1(1, true);
                            std::cout << "KEY UP: P2 up\n";
                            break;
                        case p2_down:
                            memory_mapped_io->write_in1(4, true);
                            std::cout << "KEY UP: P2 down\n";
                            break;
                        case p2_left:
                            memory_mapped_io->write_in1(2, true);
                            std::cout << "KEY UP: P2 left\n";
                            break;
                        case p2_right:
                            memory_mapped_io->write_in1(3, true);
                            std::cout << "KEY UP: P2 right\n";
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (read_input_event.key.keysym.scancode) {
                        case mute:
                            notify_io_observers(IoRequest::TOGGLE_MUTE);
                            std::cout << "KEY DOWN: Mute\n";
                            break;
                        case pause:
                            if (run_status == RunStatus::PAUSED) {
                                run_status = RunStatus::RUNNING;
                            } else if (run_status == RunStatus::RUNNING) {
                                run_status = RunStatus::PAUSED;
                            }
                            std::cout << "KEY DOWN: Pause\n";
                            break;
                        case insert_coin:
                            memory_mapped_io->write_in0(5, false);
                            std::cout << "KEY DOWN: Insert coin\n";
                            break;
                        case p1_start:
                            memory_mapped_io->write_in1(5, false);
                            std::cout << "KEY DOWN: P1 start\n";
                            break;
                        case p1_up:
                            memory_mapped_io->write_in0(1, false);
                            std::cout << "KEY DOWN: P1 up\n";
                            break;
                        case p1_down:
                            memory_mapped_io->write_in0(4, false);
                            std::cout << "KEY DOWN: P1 down\n";
                            break;
                        case p1_left:
                            memory_mapped_io->write_in0(2, false);
                            std::cout << "KEY DOWN: P1 left\n";
                            break;
                        case p1_right:
                            memory_mapped_io->write_in0(3, false);
                            std::cout << "KEY DOWN: P1 right\n";
                            break;
                        case p2_start:
                            memory_mapped_io->write_in1(6, false);
                            std::cout << "KEY DOWN: P2 start\n";
                            break;
                        case p2_up:
                            memory_mapped_io->write_in1(1, false);
                            std::cout << "KEY DOWN: P2 up\n";
                            break;
                        case p2_down:
                            memory_mapped_io->write_in1(4, false);
                            std::cout << "KEY DOWN: P2 down\n";
                            break;
                        case p2_left:
                            memory_mapped_io->write_in1(2, false);
                            std::cout << "KEY DOWN: P2 left\n";
                            break;
                        case p2_right:
                            memory_mapped_io->write_in1(3, false);
                            std::cout << "KEY DOWN: P2 right\n";
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

    void InputSdl::read_debug_only([[maybe_unused]] RunStatus &run_status) {}
}
