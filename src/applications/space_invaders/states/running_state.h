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

class RunningState : public State {

public:
    explicit RunningState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "";

    // Game loop - begin
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 2000;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    static constexpr unsigned int s_rst_1_i8080 = 0xcf;
    static constexpr unsigned int s_rst_2_i8080 = 0xd7;

    std::shared_ptr<StateContext> m_ctx;

    std::vector<u8> vram();
};

}
