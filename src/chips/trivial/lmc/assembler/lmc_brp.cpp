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

auto LmcBrp::eval() -> Data
{
    return opcode + m_operand.eval();
}

auto LmcBrp::parse(Scanner& scanner, Environment& environment) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Brp);

    LmcOperand const operand = LmcOperand::parse(scanner, environment);

    return std::make_unique<LmcBrp>(operand);
}
}
