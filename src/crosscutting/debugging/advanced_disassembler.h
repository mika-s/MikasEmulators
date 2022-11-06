#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_ADVANCED_DISASSEMBLER_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_ADVANCED_DISASSEMBLER_H

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

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_ADVANCED_DISASSEMBLER_H
