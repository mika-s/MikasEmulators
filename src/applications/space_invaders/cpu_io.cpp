#include "cpu_io.h"

namespace emu::applications::space_invaders {

    CpuIo::CpuIo(u8 in_port0, u8 in_port1, u8 in_port2)
            : m_in_port0(in_port0),
              m_in_port1(in_port1),
              m_in_port2(in_port2) {
    }

    void CpuIo::set_dipswitches(const Settings &settings) {
        if (settings.m_coin_info == CoinInfo::Off) {
            m_in_port2 |= (1U << dipswitch_coin_info);
        }

        if (settings.m_bonus_life_at == BonusLifeAt::_1000) {
            m_in_port2 |= (1U << dipswitch_bonus_life);
        }

        switch (settings.m_number_of_lives) {
            case NumberOfLives::Six:
                m_in_port2 |= 0b00000011;
                break;
            case NumberOfLives::Five:
                m_in_port2 |= 0b00000010;
                break;
            case NumberOfLives::Four:
                m_in_port2 |= 0b00000001;
                break;
            case NumberOfLives::Three:
                break;
        }
    }
}
