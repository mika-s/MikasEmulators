#pragma once

namespace emu::misc {

enum RunStatus {
    NOT_RUNNING,
    RUNNING,
    PAUSED,
    FINISHED,
    STEPPING,
};
}
