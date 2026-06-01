#pragma once

#include "crosscutting/typedefs.h"
#include "pacman/interfaces/state.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::pacman {
class StateContext;
}

namespace emu::applications::pacman {

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

    [[nodiscard]] auto tile_ram() const -> std::vector<u8>;

    [[nodiscard]] auto sprite_ram() const -> std::vector<u8>;

    [[nodiscard]] auto palette_ram() const -> std::vector<u8>;
};

}
