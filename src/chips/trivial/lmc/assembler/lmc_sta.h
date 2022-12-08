#pragma once

#include "instruction_interface.h"
#include "lmc_operand.h"
#include "usings.h"
#include <memory>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

class LmcSta : public InstructionInterface {

public:
    explicit LmcSta(LmcOperand operand);

    Data eval() override;

    static std::unique_ptr<InstructionInterface> parse(Scanner& scanner, Environment& environment);

private:
    static const inline Data opcode = Data(300);

    LmcOperand m_operand;
};
}
