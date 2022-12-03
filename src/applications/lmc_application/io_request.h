#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_IO_REQUEST_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_IO_REQUEST_H

namespace emu::applications::lmc {

    enum IoRequest {
        STEP_INSTRUCTION,
        STEP_CYCLE,
        CONTINUE_EXECUTION,
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_IO_REQUEST_H
