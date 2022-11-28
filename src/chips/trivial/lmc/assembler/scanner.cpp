#include "scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "crosscutting/util/string_util.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace emu::lmc {

    using emu::util::string::split;

    Scanner::Scanner(const std::stringstream &code)
        : m_real_line_no(0),
          m_logical_line_no(0),
          m_current_address(0) {
        m_code_lines = split(code, "\n");
    }

    void Scanner::test_scanner() {
        do {
            read_next_token();
        } while (current_token().kind() != TokenKind::Eof);
        std::cout << "\n\n"
                  << std::flush;
    }

    Address Scanner::current_address() {
        return m_current_address;
    }

    Token Scanner::current_token() {
        while (tokens_current_line.empty()) {
            read_next_line();
        }

        return tokens_current_line[0];
    }

    void Scanner::skip(TokenKind next) {
        TokenKind current = current_token().kind();
        if (current != next) {
            throw std::invalid_argument(
                    fmt::format(
                            "Skipping wrong token. Was {}, but expecting {}.",
                            TokenKind_as_string.at(current),
                            TokenKind_as_string.at(next)
                    )
            );
        }
        read_next_token();
    }

    void Scanner::read_next_token() {
        if (!tokens_current_line.empty()) {
            tokens_current_line.erase(tokens_current_line.begin());
        }
    }

    void Scanner::read_next_line() {
        m_current_address++;
        tokens_current_line.clear();

        if (m_real_line_no >= m_code_lines.size()) {
            std::cout << m_logical_line_no + 1 << ": " << std::flush;
            tokens_current_line.emplace_back(TokenKind::Eof);
        } else if (m_code_lines[m_real_line_no].empty() || is_comment_line(m_code_lines[m_real_line_no])) {
        } else {
            std::cout << m_logical_line_no + 1 << ": " << std::flush;
            read_tokens(m_code_lines[m_real_line_no]);
            tokens_current_line.emplace_back(TokenKind::Newline);
            ++m_logical_line_no;
        }

        for (Token &token: tokens_current_line) {
            std::cout << token << " " << std::flush;
        }
        std::cout << "\n"
                  << std::flush;

        ++m_real_line_no;
    }

    void Scanner::read_tokens(const std::string &line) {
        m_current_pos = 0;

        while (m_current_pos < line.length()) {
            if (!(handle_single_character(line) || handle_number(line) || handle_keyword(line))) {
                std::cerr << line << " (pos: " << m_current_pos << ")\n"
                          << std::flush;
                exit(1);
            }
        }
    }

    bool Scanner::is_comment_line(const std::string &line) {
        return line.starts_with("//");
    }

    bool Scanner::handle_single_character(const std::string &line) {
        switch (line[m_current_pos]) {
            case '\n':
                tokens_current_line.emplace_back(TokenKind::Newline);
                ++m_current_pos;
                break;
            case ' ':
                m_current_pos++;
                break;
            default:
                return false;
        }

        return true;
    }

    bool Scanner::handle_number(const std::string &line) {
        if (std::isdigit(line[m_current_pos])) {
            const std::size_t length = line.length();
            const std::size_t start = m_current_pos;

            while (m_current_pos + 1 < length && std::isdigit(line[m_current_pos + 1])) {
                ++m_current_pos;
            }

            ++m_current_pos;

            const std::string int_string = line.substr(start, m_current_pos - start);
            const int literal = std::atoi(int_string.c_str());

            tokens_current_line.emplace_back(TokenKind::Integer, literal);
            return true;
        } else {
            return false;
        }
    }

    bool Scanner::handle_keyword(const std::string &line) {
        if (std::isalpha(line[m_current_pos])) {
            const std::size_t length = line.length();
            const std::size_t start = m_current_pos;

            while (m_current_pos + 1 < length && std::isalpha(line[m_current_pos + 1])) {
                ++m_current_pos;
            }

            ++m_current_pos;

            std::string keyword = line.substr(start, m_current_pos - start);
            std::string normalized_keyword = keyword;
            std::transform(normalized_keyword.begin(), normalized_keyword.end(), normalized_keyword.begin(), ::toupper);

            if (keyword_string_as_TokenKind.contains(normalized_keyword)) {
                tokens_current_line.emplace_back(keyword_string_as_TokenKind.at(normalized_keyword));
                return true;
            } else {
                tokens_current_line.emplace_back(TokenKind::Label, keyword);
                return true;
            }
        } else {
            return false;
        }
    }
}
