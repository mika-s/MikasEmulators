#pragma once

#include "crosscutting/typedefs.h"
#include "space_invaders/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::space_invaders {
class StateContext;
}
namespace emu::i8080 {
class Cpu;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::misc {
class Governor;
}

namespace emu::applications::space_invaders {

using emu::i8080::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;

class PausedState : public State {

public:
    explicit PausedState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Paused";

    std::shared_ptr<StateContext> m_ctx;

    std::vector<u8> vram();
};

}
