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
    : m_operand(std::move(operand))
{
}

auto LmcSta::eval() -> Data
{
    return opcode + m_operand.eval();
}

auto LmcSta::parse(Scanner& scanner, Environment& environment) -> std::unique_ptr<InstructionInterface>
{
    scanner.skip(TokenKind::Sta);
    LmcOperand const operand = LmcOperand::parse(scanner, environment);

    return std::make_unique<LmcSta>(operand);
}
}
