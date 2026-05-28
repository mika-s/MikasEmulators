#pragma once

#include "crosscutting/typedefs.h"
#include "game_boy/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::game_boy {
class StateContext;
}

namespace emu::applications::game_boy {

class PausedState : public State {

public:
    explicit PausedState(std::shared_ptr<StateContext> state_context);

    auto is_exit_state() -> bool override;

    void transition_to_run() override;

    void transition_to_pause() override;

    void transition_to_stop() override;

    void transition_to_step() override;

    void perform(cyc& cycles) override;

private:
    static inline std::string s_game_window_subtitle = "Paused";

    std::shared_ptr<StateContext> m_ctx;

    [[nodiscard]] auto tile_ram_block_1() const -> std::vector<u8>;

    [[nodiscard]] auto tile_ram_block_2() const -> std::vector<u8>;

    [[nodiscard]] auto tile_ram_block_3() const -> std::vector<u8>;

    [[nodiscard]] auto tile_map_1() const -> std::vector<u8>;

    [[nodiscard]] auto tile_map_2() const -> std::vector<u8>;

    [[nodiscard]] auto sprite_ram() const -> std::vector<u8>;

    [[nodiscard]] auto palette_ram() const -> std::vector<u8>;
};

}
