#include <SDL_timer.h>

namespace emu::misc {

long double sdl_get_ticks_high_performance()
{
    return static_cast<long double>(
        1000.0L * SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency());
}
}
