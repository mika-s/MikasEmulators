#include "lmc_sta.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include "lmc_operand.h"
#include "usings.h"
#include <utility>

namespace emu::lmc {
    class Environment;
}
namespace emu::lmc {
    class InstructionInterface;
}

namespace emu::lmc {

    LmcSta::LmcSta(LmcOperand operand)
        : m_operand(std::move(operand)) {
    }

    Data LmcSta::eval() {
        return opcode + m_operand.eval();
    }

    std::unique_ptr<InstructionInterface> LmcSta::parse(Scanner &scanner, Environment &environment) {
        scanner.skip(TokenKind::Sta);
        LmcOperand operand = LmcOperand::parse(scanner, environment);

        return std::make_unique<LmcSta>(operand);
    }
}
