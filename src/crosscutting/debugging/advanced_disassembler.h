#pragma once

#include "basic_block.h"

namespace emu::debugger {

class AdvancedDisassembler {
public:
    explicit AdvancedDisassembler(std::vector<DisassembledLine> lines);

private:
    BasicBlock m_control_flow_graph_head;

    void parse_disassembled_lines(std::vector<DisassembledLine> lines);
};
}
