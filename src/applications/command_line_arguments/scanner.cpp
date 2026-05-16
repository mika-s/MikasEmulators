#include "scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace emu::applications::command_line_arguments {

Scanner::Scanner(std::vector<std::string> args)
    : m_args(std::move(args))
{
}

auto Scanner::current_token() -> Token
{
    while (m_tokens.empty()) {
        read_next_arg();
    }

    return m_tokens[0];
}

void Scanner::skip(const TokenKind next)
{
    if (TokenKind const current = current_token().kind(); current != next) {
        throw std::invalid_argument(
            std::format(
                "Skipping wrong token. Was {}, but expecting {}.",
                TokenKind_as_string.at(current),
                TokenKind_as_string.at(next)));
    }
    read_next_token();
}

auto Scanner::tokens_as_strings() -> std::vector<std::string>
{
    std::vector<std::string> tokens;
    std::transform(m_all_scanned_tokens.begin(), m_all_scanned_tokens.end(), std::back_inserter(tokens),
        [](Token const& token) -> std::string { std::stringstream ss; ss << token; return ss.str(); });

    return tokens;
}

void Scanner::read_next_token()
{
    if (!m_tokens.empty()) {
        m_tokens.erase(m_tokens.begin());
    }
}

void Scanner::read_next_arg()
{
    if (m_arg_idx >= m_args.size()) {
        m_tokens.emplace_back(TokenKind::Eof);
        m_all_scanned_tokens.emplace_back(TokenKind::Eof);
    } else {
        read_token(m_args[m_arg_idx++]);
    }
}

void Scanner::read_token(std::string const& line)
{
    m_current_pos = 0;

    while (m_current_pos < line.length()) {
        if (!(handle_single_character(line) || handle_short_option(line) || handle_long_option(line) || handle_identifier(line))) {
            throw std::invalid_argument(std::format("Unable to parse character(s) in position {} of arguments", m_current_pos));
        }
    }
}

auto Scanner::handle_single_character(std::string const& line) -> bool
{
    switch (line[m_current_pos]) {
    case '=':
        m_tokens.emplace_back(TokenKind::Equals);
        m_all_scanned_tokens.emplace_back(TokenKind::Equals);
        ++m_current_pos;
        break;
    default:
        return false;
    }

    return true;
}

auto Scanner::handle_short_option(std::string const& line) -> bool
{
    if (line[m_current_pos] == s_start_of_flag) {
        bool const is_next_hyphen = m_current_pos < line.length() - 1 && line[m_current_pos + 1] == '-';
        if (is_next_hyphen) {
            return false;
        }

        if (m_current_pos + 1 < line.length() && std::isalnum(line[m_current_pos + 1])) {
            const auto literal = std::string({ line[m_current_pos + 1] });

            assert(!literal.empty());

            m_tokens.emplace_back(TokenKind::ShortOption, literal);
            m_all_scanned_tokens.emplace_back(TokenKind::ShortOption, literal);
            m_current_pos += 2;

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

auto is_legal_long_option_literal_character(char character) -> bool
{
    const bool is_alnum = std::isalnum(static_cast<unsigned char>(character));
    const bool is_hyphen = character == '-';
    const bool is_underscore = character == '_';

    return is_alnum || is_hyphen || is_underscore;
}

auto Scanner::handle_long_option(std::string const& line) -> bool
{
    if (line[m_current_pos] == s_start_of_flag) {
        bool const is_next_hyphen = m_current_pos < line.length() - 1 && line[m_current_pos + 1] == '-';
        bool const is_next_next_alnum = m_current_pos < line.length() - 2 && std::isalnum(line[m_current_pos + 2]);
        if (!is_next_hyphen || !is_next_next_alnum) {
            return false;
        }

        ++m_current_pos;

        const std::size_t length = line.length();
        const std::size_t start = m_current_pos + 1;

        while (m_current_pos + 1 < length && is_legal_long_option_literal_character(line[m_current_pos + 1])) {
            ++m_current_pos;
        }

        ++m_current_pos;

        const std::string literal = line.substr(start, m_current_pos - start);

        assert(!literal.empty());

        m_tokens.emplace_back(TokenKind::LongOption, literal);
        m_all_scanned_tokens.emplace_back(TokenKind::LongOption, literal);

        return true;
    }

    return false;
}

auto Scanner::handle_identifier(std::string const& line) -> bool
{
    if (char const current_char = line[m_current_pos]; current_char != s_start_of_flag) {
        const std::size_t length = line.length();
        const std::size_t start = m_current_pos;

        while (m_current_pos + 1 < length) {
            ++m_current_pos;
        }

        ++m_current_pos;

        const std::string literal = line.substr(start, m_current_pos - start);
        m_tokens.emplace_back(TokenKind::Identifier, literal);
        m_all_scanned_tokens.emplace_back(TokenKind::Identifier, literal);

        return true;
    }

    return false;
}
}
