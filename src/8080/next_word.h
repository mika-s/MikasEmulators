#ifndef MIKA_EMULATORS_8080_NEXT_WORD_H
#define MIKA_EMULATORS_8080_NEXT_WORD_H

#include <cstdint>

namespace emu::cpu8080 {
    typedef struct NextWord_s {
        std::uint8_t farg;
        std::uint8_t sarg;
    } NextWord;
}

#endif //MIKA_EMULATORS_8080_NEXT_WORD_H
