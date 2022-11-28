#include "lmc_lda.h"
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

    LmcLda::LmcLda(LmcOperand operand)
        : m_operand(std::move(operand)) {
    }

    Data LmcLda::eval() {
        return opcode + m_operand.eval();
    }

    std::unique_ptr<InstructionInterface> LmcLda::parse(Scanner &scanner, Environment &environment) {
        scanner.skip(TokenKind::Lda);

        LmcOperand operand = LmcOperand::parse(scanner, environment);

        return std::make_unique<LmcLda>(operand);
    }
}
