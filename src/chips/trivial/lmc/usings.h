#ifndef MIKA_EMULATORS_CHIPS_LMC_USINGS_H
#define MIKA_EMULATORS_CHIPS_LMC_USINGS_H

#include "crosscutting/misc/uinteger.h"

namespace emu::lmc {
    using Address = emu::misc::UInteger<100>;
    using Data = emu::misc::UInteger<1000>;
}

#endif //MIKA_EMULATORS_CHIPS_LMC_USINGS_H
