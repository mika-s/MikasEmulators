#ifndef MIKA_EMULATORS_8080_NEXT_WORD_H
#define MIKA_EMULATORS_8080_NEXT_WORD_H

#include "crosscutting/typedefs.h"

namespace emu::i8080 {

    typedef struct NextWord_s {
        u8 farg;
        u8 sarg;
    } NextWord;
}

#endif //MIKA_EMULATORS_8080_NEXT_WORD_H
