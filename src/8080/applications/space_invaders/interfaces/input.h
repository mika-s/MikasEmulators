#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H

#include "8080/applications/space_invaders/io.h"

namespace emu::cpu8080::applications::space_invaders {

    class Input {
    public:
        virtual ~Input() = default;

        virtual void read(bool &is_finished, bool &is_paused, Io &cpu_io) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H
