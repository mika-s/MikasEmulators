#include "lmc_label.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "crosscutting/misc/uinteger.h"
#include "environment.h"
#include <optional>
#include <string>
#include <utility>

namespace emu::lmc {

LmcLabel::LmcLabel(std::string literal, Environment& environment)
    : m_literal(std::move(literal))
    , m_environment(environment)
{
}

Address LmcLabel::eval()
{
    return m_environment.get_address_given_label(m_literal);
}

std::optional<LmcLabel> LmcLabel::parse(Scanner& scanner, Environment& environment)
{
    Token current_token = scanner.current_token();
    if (current_token.kind() == TokenKind::Label) {
        const std::string label = current_token.label_literal();
        scanner.skip(TokenKind::Label);
        environment.add_label(label, scanner.current_address() - Address(1));
        return { LmcLabel(label, environment) };
    } else {
        return std::nullopt;
    }
}
}
