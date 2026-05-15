#include "lmc_operand.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "lmc_integer.h"
#include "lmc_label_argument.h"
#include <utility>

namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

LmcOperand::LmcOperand(
    const std::optional<LmcInteger> integer_literal,
    std::optional<LmcLabelArgument> label_literal)
    : m_integer_literal(integer_literal)
    , m_label_literal(std::move(label_literal))
{
}

auto LmcOperand::eval() const -> Data
{
    if (m_integer_literal.has_value()) {
        return m_integer_literal->eval();
    }

    return Data(m_label_literal->eval().underlying());
}

auto LmcOperand::parse(Scanner& scanner, Environment& environment) -> LmcOperand
{
    if (scanner.current_token().kind() == TokenKind::Integer) {
        LmcInteger operand = LmcInteger::parse(scanner);
        return { std::optional(operand), std::nullopt };
    }

    LmcLabelArgument operand = LmcLabelArgument::parse(scanner, environment);
    return { std::nullopt, std::optional(operand) };
}
}
