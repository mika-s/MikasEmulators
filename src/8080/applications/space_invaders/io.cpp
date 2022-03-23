#include "io.h"

namespace emu::cpu8080::applications::space_invaders {

    Io::Io(
            std::uint8_t in_port0,
            std::uint8_t in_port1,
            std::uint8_t in_port2
    )
            : in_port0(in_port0),
              in_port1(in_port1),
              in_port2(in_port2) {
    }

    void Io::set_dipswitches(const Settings &settings) {
        if (settings.coin_info == CoinInfo::Off) {
            in_port2 |= (1U << dipswitch_coin_info);
        }

        if (settings.bonus_life_at == BonusLifeAt::_1000) {
            in_port2 |= (1U << dipswitch_bonus_life);
        }

        switch (settings.number_of_lives) {
            case NumberOfLives::Six:
                in_port2 |= 0b00000011;
                break;
            case NumberOfLives::Five:
                in_port2 |= 0b00000010;
                break;
            case NumberOfLives::Four:
                in_port2 |= 0b00000001;
                break;
            case NumberOfLives::Three:
            default:
                break;
        }
    }
}
