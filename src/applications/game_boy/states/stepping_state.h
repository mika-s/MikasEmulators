#pragma once

#include "applications/game_boy/interfaces/state.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string_view>
#include <vector>

namespace emu::applications::game_boy {
class StateContext;
}

namespace emu::applications::game_boy {

class SteppingState : public State {

public:
    explicit SteppingState(std::shared_ptr<StateContext> state_context);

    auto is_exit_state() -> bool override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static constexpr std::string_view s_game_window_subtitle = "Stepping";

    // Game loop - begin
    static constexpr double s_fps = 60.0;
    static constexpr double s_tick_limit = 1000.0 / s_fps;
    static constexpr double s_cycles_per_ms = 4194.304;
    static constexpr double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    bool m_is_stepping_cycle { false };

    std::shared_ptr<StateContext> m_ctx;

    auto await_input_and_update_debug() -> bool;

    [[nodiscard]] auto tile_ram_block_1() const -> std::vector<u8>;

    [[nodiscard]] auto tile_ram_block_2() const -> std::vector<u8>;

    [[nodiscard]] auto tile_ram_block_3() const -> std::vector<u8>;

    [[nodiscard]] auto tile_map_1() const -> std::vector<u8>;

    [[nodiscard]] auto tile_map_2() const -> std::vector<u8>;

    [[nodiscard]] auto sprite_ram() const -> std::vector<u8>;

    [[nodiscard]] auto palette_ram() const -> std::vector<u8>;
};

}
