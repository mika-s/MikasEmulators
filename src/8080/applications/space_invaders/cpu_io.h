#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_CPU_IO_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_CPU_IO_H

#include "8080/shift_register.h"
#include "8080/applications/space_invaders/settings.h"

namespace emu::cpu8080::applications::space_invaders {

    class CpuIo {
    public:
        CpuIo(std::uint8_t in_port0, std::uint8_t in_port1, std::uint8_t in_port2);

        void set_dipswitches(const Settings &settings);

        std::uint8_t m_in_port0;
        std::uint8_t m_in_port1;
        std::uint8_t m_in_port2;
        ShiftRegister m_shift_register;

    private:
        static constexpr int dipswitch_bonus_life = 3;
        static constexpr int dipswitch_coin_info = 7;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_CPU_IO_H
