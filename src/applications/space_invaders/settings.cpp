#include "settings.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "options.h"
#include "usage.h"
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace emu::applications::space_invaders {

    using emu::exceptions::InvalidProgramArgumentsException;

    Settings Settings::from_options(Options &options) {
        Settings settings{
                .m_number_of_lives = NumberOfLives::Three,
                .m_bonus_life_at = BonusLifeAt::_1500,
                .m_coin_info = CoinInfo::On
        };

        std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

        for (auto opt: opts["-d"]) {
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
                        throw InvalidProgramArgumentsException(ss.str(), &print_usage);
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
                        throw InvalidProgramArgumentsException(ss.str(), &print_usage);
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
                        throw InvalidProgramArgumentsException(ss.str(), &print_usage);
                    }
                    break;
                default:
                    std::stringstream ss;
                    ss << "Unknown -d value: " << opt << "\n";
                    throw InvalidProgramArgumentsException(ss.str(), &print_usage);
            }
        }

        return settings;
    }
}
