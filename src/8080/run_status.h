#ifndef MIKA_EMULATORS_8080_RUN_STATUS_H
#define MIKA_EMULATORS_8080_RUN_STATUS_H

namespace emu::i8080 {

    enum RunStatus {
        NOT_RUNNING,
        RUNNING,
        PAUSED,
        FINISHED,
        STEPPING,
    };
}

#endif //MIKA_EMULATORS_8080_RUN_STATUS_H
