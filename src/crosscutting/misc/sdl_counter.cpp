#include <SDL_timer.h>

namespace emu::misc {

auto sdl_get_ticks_high_performance() -> double
{
    return 1000.0 * static_cast<double>(SDL_GetPerformanceCounter())
               / static_cast<double>(SDL_GetPerformanceFrequency());
    }
}
