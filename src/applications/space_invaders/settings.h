#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SETTINGS_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SETTINGS_H

#include "applications/options.h"

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

        static Settings from_options(Options &options);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SETTINGS_H
