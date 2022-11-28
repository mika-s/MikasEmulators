#include "lmc_brz.h"
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

    LmcBrz::LmcBrz(LmcOperand operand)
        : m_operand(std::move(operand)) {
    }

    Data LmcBrz::eval() {
        return opcode + m_operand.eval();
    }

    std::unique_ptr<InstructionInterface> LmcBrz::parse(Scanner &scanner, Environment &environment) {
        scanner.skip(TokenKind::Brz);

        LmcOperand operand = LmcOperand::parse(scanner, environment);

        return std::make_unique<LmcBrz>(operand);
    }
}
