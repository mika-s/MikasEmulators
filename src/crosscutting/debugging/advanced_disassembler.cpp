#include "advanced_disassembler.h"

#include <utility>

namespace emu::debugger {

AdvancedDisassembler::AdvancedDisassembler(std::vector<DisassembledLine> lines)
{
    parse_disassembled_lines(std::move(lines));
}
void AdvancedDisassembler::parse_disassembled_lines(std::vector<DisassembledLine> lines)
{
    // Find all entry points
    //  After entry point, one exit point

    m_control_flow_graph_head = BasicBlock(std::move(lines));
}
}
