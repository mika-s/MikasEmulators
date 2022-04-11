#ifndef MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H
#define MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H

#include "8080/interfaces/session.h"

namespace emu::cpu8080 {

    class Emulator8080 {
    public:
        virtual ~Emulator8080() = default;

        virtual std::unique_ptr<Session> new_session() = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_EMULATOR_8080_H
