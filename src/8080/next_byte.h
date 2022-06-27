#ifndef MIKA_EMULATORS_8080_NEXT_BYTE_H
#define MIKA_EMULATORS_8080_NEXT_BYTE_H

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

    typedef struct NextByte_s {
        u8 farg;
    } NextByte;
}

#endif //MIKA_EMULATORS_8080_NEXT_BYTE_H
