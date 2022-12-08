#pragma once

#include "session.h"
#include <memory>

namespace emu::misc {

class Emulator {
public:
    virtual ~Emulator() = default;

    virtual std::unique_ptr<Session> new_session() = 0;
};
}
