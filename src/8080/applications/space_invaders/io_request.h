#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_IO_REQUEST_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_IO_REQUEST_H

namespace emu::cpu8080::applications::space_invaders {

    enum IoRequest {
        BREAK_EXECUTION,
        CONTINUE_EXECUTION,
        STEP_OVER,
        STEP_INTO,
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_IO_REQUEST_H