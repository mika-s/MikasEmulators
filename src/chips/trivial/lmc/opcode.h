#pragma once

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
    OTC,
    HLT
};
}
