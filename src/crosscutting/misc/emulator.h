#pragma once

#include "session.h"
#include <memory>

namespace emu::misc {

class Emulator {
public:
    virtual ~Emulator() = default;

    virtual auto new_session() -> std::unique_ptr<Session> = 0;
};
}
