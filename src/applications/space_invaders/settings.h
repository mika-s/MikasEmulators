#pragma once

#include <string>
#include <unordered_set>

namespace emu::applications {
class Options;
}

namespace emu::applications::space_invaders {

enum NumberOfLives {
    Three = 0,
    Four = 1,
    Five = 2,
    Six = 3
};

enum BonusLifeAt {
    _1000 = 1,
    _1500 = 0
};

enum CoinInfo {
    On = 0,
    Off = 1
};

class Settings {
public:
    NumberOfLives m_number_of_lives;
    BonusLifeAt m_bonus_life_at;
    CoinInfo m_coin_info;

    static Settings from_options(Options const& options);

private:
    static const inline std::string s_help_short = "h";
    static const inline std::string s_help_long = "help";
    static const inline std::string s_debug_scanner_long = "debug-scanner";

    static const inline std::string s_dipswitch_short = "d";
    static const inline std::string s_gui_short = "g";

    static const inline std::unordered_set<std::string> s_recognized_options = {
        s_help_short, s_help_long, s_debug_scanner_long, s_dipswitch_short, s_gui_short
    };
};
}
