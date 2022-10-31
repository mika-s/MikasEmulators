#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_CPU_IO_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_CPU_IO_H

#include "chips/8080/shift_register.h"
#include "crosscutting/typedefs.h"
#include "settings.h"

namespace emu::applications::space_invaders {

    using emu::i8080::ShiftRegister;

    class CpuIo {
    public:
        CpuIo(u8 in_port0, u8 in_port1, u8 in_port2);

        void set_dipswitches(const Settings &settings);

        u8 m_in_port0;
        u8 m_in_port1;
        u8 m_in_port2;
        ShiftRegister m_shift_register;

    private:
        static constexpr int dipswitch_bonus_life = 3;
        static constexpr int dipswitch_coin_info = 7;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_CPU_IO_H
