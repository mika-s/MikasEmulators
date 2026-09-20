#pragma once

namespace emu::applications::game_boy {

enum class Interrupts {
    VBLANK,
    LCD,
    TIMER,
    JOYPAD,
};
}
