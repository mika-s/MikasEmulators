#pragma once

namespace emu::applications::lmc {

enum IoRequest {
    STEP_INSTRUCTION,
    STEP_CYCLE,
    CONTINUE_EXECUTION,
};
}
