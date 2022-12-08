#include "lmc_sub.h"
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

LmcSub::LmcSub(LmcOperand operand)
    : m_operand(std::move(operand))
{
}

Data LmcSub::eval()
{
    return opcode + m_operand.eval();
}

std::unique_ptr<InstructionInterface> LmcSub::parse(Scanner& scanner, Environment& environment)
{
    scanner.skip(TokenKind::Sub);

    LmcOperand operand = LmcOperand::parse(scanner, environment);

    return std::make_unique<LmcSub>(operand);
}
}
