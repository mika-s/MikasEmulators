#include "input_sdl.h"
#include "gui_io.h"
#include "interfaces/io_observer.h"
#include "io_request.h"
#include "memory_mapped_io_for_pacman.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::pacman {

void InputSdl::add_io_observer(IoObserver& observer)
{
    m_io_observers.push_back(&observer);
}

void InputSdl::remove_io_observer(IoObserver* observer)
{
    m_io_observers.erase(
        std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
        m_io_observers.end());
}

void InputSdl::notify_io_observers(IoRequest request)
{
    for (IoObserver* observer : m_io_observers) {
        observer->io_changed(request);
    }
}

void InputSdl::read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
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
            case s_mute:
                notify_io_observers(IoRequest::TOGGLE_MUTE);
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
    }
}

void InputSdl::read_debug_only([[maybe_unused]] GuiIo& gui_io) { }
}
