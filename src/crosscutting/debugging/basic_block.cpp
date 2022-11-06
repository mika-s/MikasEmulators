#include "basic_block.h"
#include <utility>

namespace emu::debugger {

    BasicBlock::BasicBlock() = default;

    BasicBlock::BasicBlock(std::vector<DisassembledLine> lines)
        : m_lines(std::move(lines)) {
    }
}
