#pragma once

#include "crosscutting/typedefs.h"
#include "zxspectrum_48k/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class StateContext;
}
namespace emu::z80 {
class Cpu;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::misc {
class Governor;
}

namespace emu::applications::zxspectrum_48k {

using emu::z80::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;

class SteppingState : public State {

public:
    explicit SteppingState(std::shared_ptr<StateContext> state_context);

    bool is_exit_state() override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Stepping";

    // Game loop - begin
    static constexpr long double s_fps = 50.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 3500;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    bool m_is_stepping_cycle { false };

    std::shared_ptr<StateContext> m_ctx;

    bool await_input_and_update_debug();

    std::vector<u8> vram();

    std::vector<u8> color_ram();
};

}
