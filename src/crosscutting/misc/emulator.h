#ifndef MIKA_EMULATORS_CROSSCUTTING_MISC_EMULATOR_H
#define MIKA_EMULATORS_CROSSCUTTING_MISC_EMULATOR_H

#include <memory>
#include "session.h"

namespace emu::misc {

    class Emulator {
    public:
        virtual ~Emulator() = default;

        virtual std::unique_ptr<Session> new_session() = 0;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MISC_EMULATOR_H
