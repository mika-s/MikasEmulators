#ifndef MIKA_EMULATORS_8080_NEXT_BYTE_H
#define MIKA_EMULATORS_8080_NEXT_BYTE_H

#include <cstdint>

namespace emu::cpu8080 {
    typedef struct NextByte_s {
        std::uint8_t farg;
    } NextByte;
}

#endif //MIKA_EMULATORS_8080_NEXT_BYTE_H
