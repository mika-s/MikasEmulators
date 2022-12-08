#pragma once

#include "scanner.h"
#include "usings.h"

namespace emu::lmc {

class InstructionInterface {
public:
    virtual ~InstructionInterface() = default;

    virtual Data eval() = 0;
};
}
