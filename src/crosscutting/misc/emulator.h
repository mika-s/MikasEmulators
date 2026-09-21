#pragma once

#include "session.h"
#include <memory>

namespace emu::misc {

class Emulator {
public:
    virtual ~Emulator() = default;

    Emulator(const Emulator&) = delete;
    auto operator=(const Emulator&) -> Emulator& = delete;
    Emulator(Emulator&&) = delete;
    auto operator=(Emulator&&) -> Emulator& = delete;

    virtual auto new_session() -> std::unique_ptr<Session> = 0;

protected:
    Emulator() = default;
};
}
