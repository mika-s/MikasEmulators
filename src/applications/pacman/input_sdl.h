#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_INPUT_SDL_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_INPUT_SDL_H

#include <SDL_scancode.h>
#include <vector>
#include "memory_mapped_io.h"
#include "io_request.h"
#include "interfaces/input.h"
#include "interfaces/io_observer.h"
#include "crosscutting/misc/run_status.h"

namespace emu::applications::pacman {

    class InputSdl : public Input {
    public:
        void read(RunStatus &run_status, std::shared_ptr<MemoryMappedIo> memory_mapped_io) override;

        void read_debug_only(RunStatus &run_status) override;

        void add_io_observer(IoObserver &observer) override;

        void remove_io_observer(IoObserver *observer) override;

    private:
        static constexpr SDL_Scancode mute = SDL_SCANCODE_M;
        static constexpr SDL_Scancode pause = SDL_SCANCODE_PAUSE;

        static constexpr SDL_Scancode insert_coin_p1 = SDL_SCANCODE_C;
        static constexpr SDL_Scancode insert_coin_p2 = SDL_SCANCODE_V;

        static constexpr SDL_Scancode p1_start = SDL_SCANCODE_RETURN;
        static constexpr SDL_Scancode p1_up = SDL_SCANCODE_W;
        static constexpr SDL_Scancode p1_down = SDL_SCANCODE_S;
        static constexpr SDL_Scancode p1_left = SDL_SCANCODE_A;
        static constexpr SDL_Scancode p1_right = SDL_SCANCODE_D;

        static constexpr SDL_Scancode p2_start = SDL_SCANCODE_RSHIFT;
        static constexpr SDL_Scancode p2_up = SDL_SCANCODE_UP;
        static constexpr SDL_Scancode p2_down = SDL_SCANCODE_DOWN;
        static constexpr SDL_Scancode p2_left = SDL_SCANCODE_LEFT;
        static constexpr SDL_Scancode p2_right = SDL_SCANCODE_RIGHT;

        std::vector<IoObserver *> m_io_observers;

        void notify_io_observers(IoRequest request);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_INPUT_SDL_H