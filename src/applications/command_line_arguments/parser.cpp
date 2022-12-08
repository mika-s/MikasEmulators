#include "parser.h"
#include "command_line_arguments/scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"
#include "identifier.h"
#include "long_option.h"
#include "options.h"
#include "short_option.h"
#include <memory>
#include <stdexcept>
#include <utility>

namespace emu::applications::command_line_arguments {

Parser::Parser()
    : m_state(ParserStates::Command)
{
}

void Parser::parse(Scanner& scanner, Options& options)
{
    try {
        while (true) {
            if (options.is_failed().first) {
                return;
            }

            switch (m_state) {
            case ParserStates::Command:
                switch (scanner.current_token().kind()) {
                case TokenKind::Equals:
                    throw std::invalid_argument("Expected an application, not =");
                case TokenKind::ShortOption:
                case TokenKind::LongOption:
                    m_state = ParserStates::MaybeOptions;
                    break;
                case TokenKind::Identifier:
                    options.set_command(Identifier::parse(scanner));
                    m_state = ParserStates::Application;
                    break;
                case TokenKind::Eof:
                    options.set_is_asking_for_help("Command has to be specified");
                    m_state = ParserStates::Finished;
                    break;
                }
                break;
            case ParserStates::Application:
                switch (scanner.current_token().kind()) {
                case TokenKind::Equals:
                    throw std::invalid_argument("Expected an application, not =");
                case TokenKind::ShortOption:
                case TokenKind::LongOption:
                    m_state = ParserStates::MaybeOptions;
                    break;
                case TokenKind::Identifier:
                    options.set_application(Identifier::parse(scanner));
                    m_state = ParserStates::MaybeOptions;
                    break;
                case TokenKind::Eof:
                    m_state = ParserStates::Finished;
                    break;
                }
                break;
            case ParserStates::MaybeOptions:
                switch (scanner.current_token().kind()) {
                case TokenKind::ShortOption:
                    ShortOption::parse(scanner, options);
                    break;
                case TokenKind::LongOption:
                    LongOption::parse(scanner, options);
                    break;
                case TokenKind::Identifier:
                    m_state = ParserStates::MaybePath;
                    break;
                case TokenKind::Eof:
                    m_state = ParserStates::Finished;
                    break;
                default:
                    throw std::invalid_argument("Something went wrong while parsing options");
                }
                break;
            case ParserStates::MaybePath:
                switch (scanner.current_token().kind()) {
                case TokenKind::Equals:
                case TokenKind::ShortOption:
                case TokenKind::LongOption:
                    throw std::invalid_argument("Expected path to a program, not options");
                case TokenKind::Identifier:
                    options.set_path(Identifier::parse(scanner));
                    m_state = ParserStates::Finished;
                    break;
                case TokenKind::Eof:
                    m_state = ParserStates::Finished;
                    break;
                }
                break;
            case ParserStates::Finished:
                scanner.skip(TokenKind::Eof);
                return;
            }
        }
    } catch (std::invalid_argument& ex) {
        options.fail(ex.what());
    }
}
}
