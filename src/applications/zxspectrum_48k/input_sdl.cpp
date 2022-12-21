#include "input_sdl.h"
#include "crosscutting/util/byte_util.h"
#include "gui_io.h"
#include "interfaces/key_observer.h"
#include "key_request.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <algorithm>

namespace emu::applications::zxspectrum_48k {
class CpuIo;
}

namespace emu::applications::zxspectrum_48k {

using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;

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

void InputSdl::notify_io_observers(KeyRequest request)
{
    for (KeyObserver* observer : m_io_observers) {
        observer->key_pressed(request);
    }
}

void InputSdl::read([[maybe_unused]] CpuIo& cpu_io, GuiIo& gui_io)
{
    SDL_Event read_input_event;

    while (SDL_PollEvent(&read_input_event) != 0) {
        switch (read_input_event.type) {
        case SDL_QUIT:
            gui_io.m_is_quitting = true;
            break;
        case SDL_KEYUP:
            switch (read_input_event.key.keysym.scancode) {
            default:
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (read_input_event.key.keysym.scancode) {
            case s_mute:
                notify_io_observers(TOGGLE_MUTE);
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

void InputSdl::read_debug_only([[maybe_unused]] CpuIo& cpu_io, [[maybe_unused]] GuiIo& gui_io) { }
}
