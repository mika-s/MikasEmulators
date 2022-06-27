#ifndef MIKA_EMULATORS_Z80_RUN_STATUS_H
#define MIKA_EMULATORS_Z80_RUN_STATUS_H

namespace emu::z80 {

    enum RunStatus {
        NOT_RUNNING,
        RUNNING,
        PAUSED,
        FINISHED,
        STEPPING,
    };
}

#endif //MIKA_EMULATORS_Z80_RUN_STATUS_H
