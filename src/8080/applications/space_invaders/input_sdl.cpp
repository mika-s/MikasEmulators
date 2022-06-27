#include <algorithm>
#include <SDL.h>
#include "imgui_impl_sdl.h"
#include "input_sdl.h"
#include "crosscutting/util/byte_util.h"

namespace emu::i8080::applications::space_invaders {

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
            2	tilt 'button'
            3	dipswitch bonus life at 1:1000,0:1500
            4	P2 shoot button
            5	P2 joystick left
            6	P2 joystick right
            7	dipswitch coin info 1:off,0:on
    */
    void InputSdl::read(RunStatus &run_status, CpuIo &cpu_io) {
        SDL_Event read_input_event;

        while (SDL_PollEvent(&read_input_event) != 0) {
            switch (read_input_event.type) {
                case SDL_QUIT:
                    run_status = RunStatus::NOT_RUNNING;
                    break;
                case SDL_KEYUP:
                    switch (read_input_event.key.keysym.scancode) {
                        case insert_coin:
                            unset_bit(cpu_io.m_in_port1, 0);
                            break;
                        case tilt:
                            unset_bit(cpu_io.m_in_port2, 2);
                            break;
                        case p1_start:
                            unset_bit(cpu_io.m_in_port1, 2);
                            break;
                        case p1_shoot:
                            unset_bit(cpu_io.m_in_port1, 4);
                            break;
                        case p1_left:
                            unset_bit(cpu_io.m_in_port1, 5);
                            break;
                        case p1_right:
                            unset_bit(cpu_io.m_in_port1, 6);
                            break;
                        case p2_start:
                            unset_bit(cpu_io.m_in_port1, 1);
                            break;
                        case p2_shoot:
                            unset_bit(cpu_io.m_in_port2, 4);
                            break;
                        case p2_left:
                            unset_bit(cpu_io.m_in_port2, 5);
                            break;
                        case p2_right:
                            unset_bit(cpu_io.m_in_port2, 6);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (read_input_event.key.keysym.scancode) {
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
                        case insert_coin:
                            set_bit(cpu_io.m_in_port1, 0);
                            break;
                        case tilt:
                            set_bit(cpu_io.m_in_port2, 2);
                            break;
                        case p1_start:
                            set_bit(cpu_io.m_in_port1, 2);
                            break;
                        case p1_shoot:
                            set_bit(cpu_io.m_in_port1, 4);
                            break;
                        case p1_left:
                            set_bit(cpu_io.m_in_port1, 5);
                            break;
                        case p1_right:
                            set_bit(cpu_io.m_in_port1, 6);
                            break;
                        case p2_start:
                            set_bit(cpu_io.m_in_port1, 1);
                            break;
                        case p2_shoot:
                            set_bit(cpu_io.m_in_port2, 4);
                            break;
                        case p2_left:
                            set_bit(cpu_io.m_in_port2, 5);
                            break;
                        case p2_right:
                            set_bit(cpu_io.m_in_port2, 6);
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
