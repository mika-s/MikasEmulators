#include <SDL_timer.h>

namespace emu::misc {

auto sdl_get_ticks_high_performance() -> long double
{
    return static_cast<long double>(
        1000.0L * SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency());
}
}
