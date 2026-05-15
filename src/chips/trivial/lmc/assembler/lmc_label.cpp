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

auto LmcLabel::eval() const -> Address
{
    return m_environment.get_address_given_label(m_literal);
}

auto LmcLabel::parse(Scanner& scanner, Environment& environment) -> std::optional<LmcLabel>
{
    if (Token const current_token = scanner.current_token(); current_token.kind() == TokenKind::Label) {
        const std::string label = current_token.label_literal();
        scanner.skip(TokenKind::Label);
        environment.add_label(label, scanner.current_address() - Address(1));
        return { LmcLabel(label, environment) };
    }

    return std::nullopt;

}
}
