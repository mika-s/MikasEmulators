#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INPUT_IMGUI_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INPUT_IMGUI_H

#include "crosscutting/misc/run_status.h"
#include "interfaces/input.h"
#include "io_request.h"
#include <SDL_scancode.h>
#include <vector>

namespace emu::applications::lmc {
    class IoObserver;
}

namespace emu::applications::lmc {

    using emu::misc::RunStatus;

    class InputImgui : public Input {
    public:
        void read(RunStatus &run_status) override;

        void read_debug_only(RunStatus &run_status) override;

        void add_io_observer(IoObserver &observer) override;

        void remove_io_observer(IoObserver *observer) override;

    private:
        static constexpr SDL_Scancode pause = SDL_SCANCODE_PAUSE;
        static constexpr SDL_Scancode step_instruction = SDL_SCANCODE_F7;
        static constexpr SDL_Scancode step_cycle = SDL_SCANCODE_F8;
        static constexpr SDL_Scancode continue_running = SDL_SCANCODE_F9;

        std::vector<IoObserver *> m_io_observers;

        void notify_io_observers(IoRequest request);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INPUT_IMGUI_H
