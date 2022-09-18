#include "cpu_io.h"

namespace emu::z80::applications::pacman {

    CpuIo::CpuIo(u8 in_port0, u8 in_port1, u8 in_port2)
            : m_in_port0(in_port0),
              m_in_port1(in_port1),
              m_in_port2(in_port2) {
    }

    void CpuIo::set_dipswitches(const Settings &settings) {
        // TODO: Fix this

        if (settings.m_coins_per_game == CoinsPerGame::FreePlay) {
            m_in_port2 |= (1U << dipswitch_coin_info);
        }

        switch (settings.m_bonus_life_at) {
            case BonusLifeAt::_10000:
                m_in_port2 |= 0b00000011;
                break;
            case BonusLifeAt::_15000:
                m_in_port2 |= 0b00000010;
                break;
            case BonusLifeAt::_20000:
                m_in_port2 |= 0b00000001;
                break;
            case BonusLifeAt::None:
            default:
                break;
        }

        switch (settings.m_number_of_lives) {
            case NumberOfLives::One:
                m_in_port2 |= 0b00000011;
                break;
            case NumberOfLives::Two:
                m_in_port2 |= 0b00000010;
                break;
            case NumberOfLives::Three:
                m_in_port2 |= 0b00000001;
                break;
            case NumberOfLives::Five:
            default:
                break;
        }
    }
}
