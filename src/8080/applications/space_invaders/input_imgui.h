#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H

#include <SDL_scancode.h>
#include "8080/applications/space_invaders/io.h"
#include "8080/applications/space_invaders/interfaces/input.h"

namespace emu::cpu8080::applications::space_invaders {

    class InputImgui: public Input {
    public:
        void read(bool &is_finished, bool &is_paused, Io &cpu_io) override;

    private:
        static constexpr SDL_Scancode pause = SDL_SCANCODE_PAUSE;

        static constexpr SDL_Scancode insert_coin = SDL_SCANCODE_C;
        static constexpr SDL_Scancode tilt = SDL_SCANCODE_T;

        static constexpr SDL_Scancode p1_start = SDL_SCANCODE_RETURN;
        static constexpr SDL_Scancode p1_shoot = SDL_SCANCODE_W;
        static constexpr SDL_Scancode p1_left = SDL_SCANCODE_A;
        static constexpr SDL_Scancode p1_right = SDL_SCANCODE_D;

        static constexpr SDL_Scancode p2_start = SDL_SCANCODE_RSHIFT;
        static constexpr SDL_Scancode p2_shoot = SDL_SCANCODE_UP;
        static constexpr SDL_Scancode p2_left = SDL_SCANCODE_LEFT;
        static constexpr SDL_Scancode p2_right = SDL_SCANCODE_RIGHT;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H
