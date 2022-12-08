#pragma once

namespace emu::z80 {

enum class InterruptMode {
    ZERO, // 8080 compatibility mode
    ONE,
    TWO
};
}
