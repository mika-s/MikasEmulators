#pragma once

namespace emu::applications {
class Options;
}

namespace emu::applications::zxspectrum_48k {

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
};
}
