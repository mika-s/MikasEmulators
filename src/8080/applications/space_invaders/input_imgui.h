#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H

#include <SDL_scancode.h>
#include <vector>
#include "8080/run_status.h"
#include "8080/applications/space_invaders/cpu_io.h"
#include "8080/applications/space_invaders/io_request.h"
#include "8080/applications/space_invaders/interfaces/input.h"
#include "8080/applications/space_invaders/interfaces/io_observer.h"

namespace emu::cpu8080::applications::space_invaders {

    class InputImgui: public Input {
    public:
        void read(RunStatus &run_status, CpuIo &cpu_io) override;

        void add_io_observer(IoObserver &observer) override;

        void remove_io_observer(IoObserver *observer) override;

    private:
        static constexpr SDL_Scancode pause = SDL_SCANCODE_PAUSE;
        static constexpr SDL_Scancode break_program = SDL_SCANCODE_B;
        static constexpr SDL_Scancode step_into = SDL_SCANCODE_F7;
        static constexpr SDL_Scancode step_over = SDL_SCANCODE_F8;
        static constexpr SDL_Scancode continue_running = SDL_SCANCODE_F9;

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

        std::vector<IoObserver *> m_io_observers;

        void notify_io_observers(IoRequest request);
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INPUT_IMGUI_H
