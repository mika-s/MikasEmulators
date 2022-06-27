#ifndef MIKA_EMULATORS_Z80_NEXT_WORD_H
#define MIKA_EMULATORS_Z80_NEXT_WORD_H

#include "crosscutting/typedefs.h"

namespace emu::z80 {

    typedef struct NextWord_s {
        u8 farg;
        u8 sarg;
    } NextWord;
}

#endif //MIKA_EMULATORS_Z80_NEXT_WORD_H
