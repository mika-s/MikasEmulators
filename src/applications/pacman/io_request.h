#pragma once

namespace emu::applications::pacman {

enum IoRequest {
    STEP_INSTRUCTION,
    STEP_CYCLE,
    CONTINUE_EXECUTION,
    TOGGLE_MUTE,
    TOGGLE_TILE_DEBUG,
    TOGGLE_SPRITE_DEBUG,
};
}
