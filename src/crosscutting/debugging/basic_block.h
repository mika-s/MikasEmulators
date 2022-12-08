#pragma once

#include "disassembled_line.h"
#include <vector>

namespace emu::debugger {

class BasicBlock {
public:
    BasicBlock();

    explicit BasicBlock(std::vector<DisassembledLine> lines);

private:
    std::vector<DisassembledLine> m_lines;
};
}
