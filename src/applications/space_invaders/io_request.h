#pragma once

namespace emu::applications::space_invaders {

enum IoRequest {
    STEP_INSTRUCTION,
    STEP_CYCLE,
    CONTINUE_EXECUTION,
    TOGGLE_MUTE,
};
}
