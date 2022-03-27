#ifndef MIKA_EMULATORS_8080_RUN_STATUS_H
#define MIKA_EMULATORS_8080_RUN_STATUS_H

namespace emu::cpu8080 {
    enum RunStatus {
        NOT_RUNNING,
        RUNNING,
        PAUSED,
    };
}

#endif //MIKA_EMULATORS_8080_RUN_STATUS_H
