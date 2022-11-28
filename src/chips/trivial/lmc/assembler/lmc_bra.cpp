#include "lmc_bra.h"
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

    LmcBra::LmcBra(LmcOperand operand)
        : m_operand(std::move(operand)) {
    }

    Data LmcBra::eval() {
        return opcode + m_operand.eval();
    }

    std::unique_ptr<InstructionInterface> LmcBra::parse(Scanner &scanner, Environment &environment) {
        scanner.skip(TokenKind::Bra);

        LmcOperand operand = LmcOperand::parse(scanner, environment);

        return std::make_unique<LmcBra>(operand);
    }
}
