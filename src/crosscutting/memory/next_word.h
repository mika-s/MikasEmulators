#ifndef MIKA_EMULATORS_CROSSCUTTING_MEMORY_NEXT_WORD_H
#define MIKA_EMULATORS_CROSSCUTTING_MEMORY_NEXT_WORD_H

#include "crosscutting/typedefs.h"

namespace emu::memory {

    using NextWord = struct NextWord_s {
        u8 farg;
        u8 sarg;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MEMORY_NEXT_WORD_H
