#include "lmc_brp.h"
#include "assembler/scanner.h"
#include "assembler/token_kind.h"
#include "lmc_operand.h"
#include "usings.h"
#include <memory>
#include <utility>

namespace emu::lmc {
class Environment;
}
namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcBrp::LmcBrp(LmcOperand operand)
    : m_operand(std::move(operand))
{
}

Data LmcBrp::eval()
{
    return opcode + m_operand.eval();
}

std::unique_ptr<InstructionInterface> LmcBrp::parse(Scanner& scanner, Environment& environment)
{
    scanner.skip(TokenKind::Brp);

    LmcOperand operand = LmcOperand::parse(scanner, environment);

    return std::make_unique<LmcBrp>(operand);
}
}
