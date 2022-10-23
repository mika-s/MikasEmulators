#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_SETTINGS_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_SETTINGS_H

namespace emu::applications::pacman {

    enum class NumberOfLives {
        One = 0, Two = 1, Three = 2, Five = 3
    };

    enum class BonusLifeAt {
        _10000 = 0, _15000 = 1, _20000 = 2, None = 3
    };

    enum class CoinsPerGame {
        FreePlay = 0, _1C1G = 1, _1C2G = 2, _2C1G = 3
    };

    enum class Difficulty {
        Hard = 0, Normal = 1
    };

    enum class GhostNames {
        Alternate = 0, Normal = 1
    };

    enum class BoardTest {
        On = 0, Off = 1
    };

    enum class CabinetMode {
        Table = 0, Upright = 1
    };

    class Settings {
    public:
        NumberOfLives m_number_of_lives;
        BonusLifeAt m_bonus_life_at;
        CoinsPerGame m_coins_per_game;
        Difficulty m_difficulty;
        GhostNames m_ghost_names;
        BoardTest m_board_test;
        CabinetMode m_cabinet_mode;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_SETTINGS_H
