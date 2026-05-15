#pragma once

#include "scanner.h"
#include "usings.h"

namespace emu::lmc {

class InstructionInterface {
public:
    virtual ~InstructionInterface() = default;

    virtual auto eval() -> Data = 0;
};
}
