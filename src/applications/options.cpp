#include "options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include <sstream>

namespace emu::applications {

    using emu::exceptions::InvalidProgramArgumentsException;

    Options::Options(std::vector<std::string> args)
        : m_args(args) {
        for (std::size_t arg_idx = 0; arg_idx < args.size(); ++arg_idx) {
            if (args[arg_idx] == dipswitch_flag) {
                parse_flag(dipswitch_flag, args, arg_idx);
            } else if (args[arg_idx] == gui_flag) {
                parse_flag(gui_flag, args, arg_idx);
            }
        }
    }

    std::vector<std::string> Options::args() {
        return m_args;
    }

    GuiType Options::gui_type() {
        if (!m_options.contains("-g")) {
            return GuiType::ORDINARY;
        }
        if (m_options["-g"].size() > 1) {
            throw InvalidProgramArgumentsException("-g flag should only be used once");
        } else if (m_options["-g"].empty()) {
            throw InvalidProgramArgumentsException("-g flag needs an additional argument");
        }

        if (m_options["-g"][0] == "ordinary") {
            return GuiType::ORDINARY;
        } else if (m_options["-g"][0] == "debugging") {
            return GuiType::DEBUGGING;
        } else {
            throw InvalidProgramArgumentsException("Unknown GUI type passed to the -g flag");
        }
    }

    applications::pacman::Settings Options::pacman_settings() {
        using namespace applications::pacman;

        Settings settings{
                .m_number_of_lives = NumberOfLives::Three,
                .m_bonus_life_at = BonusLifeAt::_15000,
                .m_coins_per_game = CoinsPerGame::_1C1G,
                .m_difficulty = Difficulty::Normal,
                .m_ghost_names = GhostNames::Normal,
                .m_board_test = BoardTest::Off,
                .m_cabinet_mode = CabinetMode::Upright};

        for (auto &opt: m_options["-d"]) {
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
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
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                default:
                    std::stringstream ss;
                    ss << "Unknown -d value: " << opt << "\n";
                    throw InvalidProgramArgumentsException(ss.str());
            }
        }

        return settings;
    }

    applications::space_invaders::Settings Options::space_invaders_settings() {
        using namespace applications::space_invaders;

        Settings settings{
                .m_number_of_lives = NumberOfLives::Three,
                .m_bonus_life_at = BonusLifeAt::_1500,
                .m_coin_info = CoinInfo::On};

        for (auto &opt: m_options["-d"]) {
            switch (opt[0]) {
                case 'n':
                    if (opt == "n=3") {
                        settings.m_number_of_lives = NumberOfLives::Three;
                    } else if (opt == "n=4") {
                        settings.m_number_of_lives = NumberOfLives::Four;
                    } else if (opt == "n=5") {
                        settings.m_number_of_lives = NumberOfLives::Five;
                    } else if (opt == "n=6") {
                        settings.m_number_of_lives = NumberOfLives::Six;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid number of lives passed to the -d option: " << opt
                           << R"(. Should be "-d n=3", ..., "-d n=6".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'b':
                    if (opt == "b=1500") {
                        settings.m_bonus_life_at = BonusLifeAt::_1500;
                    } else if (opt == "b=1000") {
                        settings.m_bonus_life_at = BonusLifeAt::_1000;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid bonus life at passed to the -d option: " << opt
                           << R"(. Should be "-d b=1500" or "-d b=1000".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                case 'c':
                    if (opt == "c=on") {
                        settings.m_coin_info = CoinInfo::On;
                    } else if (opt == "c=off") {
                        settings.m_coin_info = CoinInfo::Off;
                    } else {
                        std::stringstream ss;
                        ss << "Invalid coin info passed to the -d option: " << opt
                           << R"(. Should be "-d c=on" or "-d c=off".)";
                        throw InvalidProgramArgumentsException(ss.str());
                    }
                    break;
                default:
                    std::stringstream ss;
                    ss << "Unknown -d value: " << opt << "\n";
                    throw InvalidProgramArgumentsException(ss.str());
            }
        }

        return settings;
    }

    void Options::parse_flag(const std::string &flag, std::vector<std::string> args, std::size_t arg_idx) {
        if (arg_idx == args.size() - 1) {
            throw InvalidProgramArgumentsException(
                    flag + " flag at the end of the line, without a value."
            );
        }

        if (m_options.count(flag) == 0) {
            std::vector<std::string> vec;
            vec.emplace_back(args[arg_idx + 1]);
            m_options[flag] = vec;
        } else {
            m_options[flag].push_back(args[arg_idx + 1]);
        }
    }
}
