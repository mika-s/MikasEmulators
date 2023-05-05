#include "settings.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "options.h"
#include "usage.h"
#include <fmt/core.h>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications::game_boy {

using emu::exceptions::InvalidProgramArgumentsException;

Settings Settings::from_options(Options const& options)
{
    using namespace applications::game_boy;

    for (auto const& opt : options.options()) {
        if (!s_recognized_options.contains(opt.first)) {
            throw InvalidProgramArgumentsException(fmt::format("Unknown flag: {}", opt.first), print_usage);
        }
    }

    Settings settings {
        .m_number_of_lives = NumberOfLives::Three,
        .m_bonus_life_at = BonusLifeAt::_15000,
        .m_coins_per_game = CoinsPerGame::_1C1G,
        .m_difficulty = Difficulty::Normal,
        .m_ghost_names = GhostNames::Normal,
        .m_board_test = BoardTest::Off,
        .m_cabinet_mode = CabinetMode::Upright
    };

    std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

    for (auto& opt : opts["d"]) {
        switch (opt[0]) {
        case 'n':
            if (opt == "n=1") {
                settings.m_number_of_lives = NumberOfLives::One;
            } else if (opt == "n=2") {
                settings.m_number_of_lives = NumberOfLives::Two;
            } else if (opt == "n=3") {
                settings.m_number_of_lives = NumberOfLives::Three;
            } else if (opt == "n=5") {
                settings.m_number_of_lives = NumberOfLives::Five;
            } else {
                std::stringstream ss;
                ss << "Invalid number of lives passed to the -d option: " << opt
                   << R"(. Should be "-d n=1", "-d n=2", "-d n=3" or "-d n=5".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 'b':
            if (opt == "b=10000") {
                settings.m_bonus_life_at = BonusLifeAt::_10000;
            } else if (opt == "b=15000") {
                settings.m_bonus_life_at = BonusLifeAt::_15000;
            } else if (opt == "b=20000") {
                settings.m_bonus_life_at = BonusLifeAt::_20000;
            } else if (opt == "b=none") {
                settings.m_bonus_life_at = BonusLifeAt::None;
            } else {
                std::stringstream ss;
                ss << "Invalid bonus life at passed to the -d option: " << opt
                   << R"(. Should be "-d b=10000", "-d b=15000", "-d b=20000" or "-d b=none".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 'c':
            if (opt == "c=1C1G") {
                settings.m_coins_per_game = CoinsPerGame::_1C1G;
            } else if (opt == "c=1C2G") {
                settings.m_coins_per_game = CoinsPerGame::_1C2G;
            } else if (opt == "c=2C1G") {
                settings.m_coins_per_game = CoinsPerGame::_2C1G;
            } else if (opt == "c=free") {
                settings.m_coins_per_game = CoinsPerGame::FreePlay;
            } else {
                std::stringstream ss;
                ss << "Invalid number of coins per game passed to the -d option: " << opt
                   << R"(. Should be "-d c=1C1G" (one per game), "-d c=1C2G" (one per two games), )"
                   << R"("-d c=2C1G" (two per game) or "-d c=free".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 'd':
            if (opt == "d=normal") {
                settings.m_difficulty = Difficulty::Normal;
            } else if (opt == "d=hard") {
                settings.m_difficulty = Difficulty::Hard;
            } else {
                std::stringstream ss;
                ss << "Invalid difficulty passed to the -d option: " << opt
                   << R"(. Should be "-d d=normal" or "-d d=hard".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 'g':
            if (opt == "g=normal") {
                settings.m_ghost_names = GhostNames::Normal;
            } else if (opt == "g=alternate") {
                settings.m_ghost_names = GhostNames::Alternate;
            } else {
                std::stringstream ss;
                ss << "Invalid ghost names passed to the -d option: " << opt
                   << R"(. Should be "-d g=normal" or "-d g=alternate".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 't':
            if (opt == "t=off") {
                settings.m_board_test = BoardTest::Off;
            } else if (opt == "t=on") {
                settings.m_board_test = BoardTest::On;
            } else {
                std::stringstream ss;
                ss << "Invalid board test passed to the -d option: " << opt
                   << R"(. Should be "-d t=off" or "-d t=on".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        case 'm':
            if (opt == "m=table") {
                settings.m_cabinet_mode = CabinetMode::Table;
            } else if (opt == "m=upright") {
                settings.m_cabinet_mode = CabinetMode::Upright;
            } else {
                std::stringstream ss;
                ss << "Invalid cabinet mode passed to the -d option: " << opt
                   << R"(. Should be "-d m=table" or "-d m=upright".)";
                throw InvalidProgramArgumentsException(ss.str(), print_usage);
            }
            break;
        default:
            std::stringstream ss;
            ss << "Unknown -d value: " << opt << "\n";
            throw InvalidProgramArgumentsException(ss.str(), print_usage);
        }
    }

    return settings;
}
}
