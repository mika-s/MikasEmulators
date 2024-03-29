#pragma once

#include "applications/game_boy/interfaces/state.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::game_boy {
class StateContext;
}

namespace emu::applications::game_boy {

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
    static constexpr long double s_cycles_per_ms = 4194.304;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    std::shared_ptr<StateContext> m_ctx;

    void update_graphics(cyc cycles);

    std::vector<u8> tile_ram_block_1();

    std::vector<u8> tile_ram_block_2();

    std::vector<u8> tile_ram_block_3();

    std::vector<u8> tile_map_1();

    std::vector<u8> tile_map_2();

    std::vector<u8> sprite_ram();

    std::vector<u8> palette_ram();
};

}
