#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_CPU_IO_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_CPU_IO_H

#include "z80/applications/pacman/settings.h"
#include "crosscutting/typedefs.h"

namespace emu::z80::applications::pacman {

    class CpuIo {
    public:
        CpuIo(u8 in_port0, u8 in_port1, u8 in_port2);

        void set_dipswitches(const Settings &settings);

        u8 m_in_port0;
        u8 m_in_port1;
        u8 m_in_port2;

    private:
        static constexpr int dipswitch_bonus_life = 3;
        static constexpr int dipswitch_coin_info = 7;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_CPU_IO_H
