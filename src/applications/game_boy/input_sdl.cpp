#include "input_sdl.h"
#include "gui_io.h"
#include "interfaces/interrupt_observer.h"
#include "interfaces/key_observer.h"
#include "interrupts.h"
#include "key_request.h"
#include "memory_mapped_io_for_game_boy.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::game_boy {

void InputSdl::add_io_observer(KeyObserver& observer)
{
    m_io_observers.push_back(&observer);
}

void InputSdl::remove_io_observer(KeyObserver* observer)
{
    m_io_observers.erase(
        std::remove(m_io_observers.begin(), m_io_observers.end(), observer),
        m_io_observers.end());
}

void InputSdl::notify_io_observers(IoRequest request)
{
    for (KeyObserver* observer : m_io_observers) {
        observer->key_pressed(request);
    }
}

void InputSdl::add_interrupt_observer(InterruptObserver& observer)
{
    m_interrupt_observers.push_back(&observer);
}

void InputSdl::remove_interrupt_observer(InterruptObserver* observer)
{
    m_interrupt_observers.erase(
        std::remove(m_interrupt_observers.begin(), m_interrupt_observers.end(), observer),
        m_interrupt_observers.end());
}

void InputSdl::read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        switch (read_input_event.type) {
        case SDL_QUIT:
            gui_io.m_is_quitting = true;
            break;
        case SDL_KEYUP:
            switch (read_input_event.key.keysym.scancode) {
            case s_p1_start:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(3, false);
                break;
            case s_p1_select:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(2, false);
                break;
            case s_up:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(0, false);
                break;
            case s_down:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(3, false);
                break;
            case s_left:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(1, false);
                break;
            case s_right:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(0, false);
                break;
            case s_a:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(1, false);
                break;
            case s_b:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(0, false);
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
            case s_p1_start:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(3, true);
                break;
            case s_p1_select:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(2, true);
                break;
            case s_up:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(0, true);
                break;
            case s_down:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(3, true);
                break;
            case s_left:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(1, true);
                break;
            case s_right:
                memory_mapped_io->p1_button_keys(5, false);
                memory_mapped_io->p1_button_keys(4, true);
                memory_mapped_io->p1_button_keys(0, true);
                break;
            case s_a:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(1, true);
                break;
            case s_b:
                memory_mapped_io->p1_button_keys(5, true);
                memory_mapped_io->p1_button_keys(4, false);
                memory_mapped_io->p1_button_keys(0, true);
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

void InputSdl::notify_interrupt_observers(Interrupts interrupt)
{
    for (InterruptObserver* observer : m_interrupt_observers) {
        observer->interrupt(interrupt);
    }
}

}
