#include "lmc_dat.h"
#include "assembler/lmc_integer.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include <memory>

namespace emu::lmc {
class InstructionInterface;
}

namespace emu::lmc {

LmcDat::LmcDat(std::optional<LmcInteger> operand)
    : m_operand(operand)
{
}

Data LmcDat::eval()
{
    if (m_operand.has_value()) {
        return m_operand->eval();
    } else {
        return Data(0);
    }
}

std::unique_ptr<InstructionInterface> LmcDat::parse(Scanner& scanner)
{
    scanner.skip(TokenKind::Dat);

    std::optional<LmcInteger> operand;
    if (scanner.current_token().kind() == TokenKind::Integer) {
        operand = std::optional(LmcInteger::parse(scanner));
    } else {
        operand = std::nullopt;
    }

    return std::make_unique<LmcDat>(operand);
}
}
