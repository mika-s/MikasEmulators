#pragma once

namespace emu::applications::game_boy {

enum Interrupts {
    VBLANK,
    LCD,
    TIMER,
    JOYPAD,
};
}
