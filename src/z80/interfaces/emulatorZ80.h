#ifndef MIKA_EMULATORS_Z80_INTERFACES_EMULATOR_Z80_H
#define MIKA_EMULATORS_Z80_INTERFACES_EMULATOR_Z80_H

#include "z80/interfaces/session.h"

namespace emu::z80 {

    class EmulatorZ80 {
    public:
        virtual ~EmulatorZ80() = default;

        virtual std::unique_ptr<Session> new_session() = 0;
    };
}

#endif //MIKA_EMULATORS_Z80_INTERFACES_EMULATOR_Z80_H
