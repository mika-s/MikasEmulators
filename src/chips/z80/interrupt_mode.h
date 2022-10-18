#ifndef MIKA_EMULATOR_CHIPS_Z80_INTERRUPT_MODE_H
#define MIKA_EMULATOR_CHIPS_Z80_INTERRUPT_MODE_H

namespace emu::z80 {

    enum class InterruptMode {
        ZERO,       // 8080 compatibility mode
        ONE,
        TWO
    };
}

#endif //MIKA_EMULATOR_CHIPS_Z80_INTERRUPT_MODE_H
