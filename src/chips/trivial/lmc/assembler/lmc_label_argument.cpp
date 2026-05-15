#include "lmc_label_argument.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "environment.h"
#include <string>
#include <utility>

namespace emu::lmc {

LmcLabelArgument::LmcLabelArgument(std::string literal, Environment& environment)
    : m_literal(std::move(literal))
    , m_environment(environment)
{
}

auto LmcLabelArgument::eval() const -> Address
{
    return m_environment.get_address_given_label(m_literal);
}

auto LmcLabelArgument::parse(Scanner& scanner, Environment& environment) -> LmcLabelArgument
{
    Token const current_token = scanner.current_token();
    const std::string label = current_token.label_literal();
    scanner.skip(TokenKind::Label);

    return { LmcLabelArgument(label, environment) };
}
}
