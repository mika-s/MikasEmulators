#ifndef MIKA_EMULATORS_CHIPS_LMC_OPCODES_H
#define MIKA_EMULATORS_CHIPS_LMC_OPCODES_H

namespace emu::lmc {

    enum class Opcode {
        ADD,
        SUB,
        STA,
        LDA,
        BRA,
        BRZ,
        BRP,
        INP,
        OUT,
        HLT
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_OPCODES_H
