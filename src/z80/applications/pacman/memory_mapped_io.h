#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_FOR_PACMAN_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_FOR_PACMAN_H

#include "z80/emulator_memory.h"
#include "z80/applications/pacman/settings.h"
#include "z80/interfaces/memory_observer.h"
#include "crosscutting/typedefs.h"

namespace emu::z80::applications::pacman {

    using emu::z80::EmulatorMemory;

    class MemoryMappedIo : public MemoryObserver {
    public:
        explicit MemoryMappedIo(EmulatorMemory &memory);

        bool is_interrupt_enabled();

        bool read_in0(unsigned int bit_number);

        void write_in0(unsigned int bit_number, bool is_setting);

        void write_in1(unsigned int bit_number, bool is_setting);

        void set_dipswitches(const Settings &settings);

        void memory_changed(u16 address) override;

        void set_initial_values();

    private:
        static constexpr u16 from_address = 0x5000;
        static constexpr u16 to_address = 0x50ff;
        static constexpr u16 address_in0 = 0x5000;
        static constexpr u16 address_sound_enable = 0x5001;
        static constexpr u16 address_flip_screen = 0x5003;
        static constexpr u16 address_in1 = 0x5040;
        static constexpr u16 address_dipswitches = 0x5080;
        static constexpr unsigned int dipswitches_coinage_1 = 0;
        static constexpr unsigned int dipswitches_coinage_2 = 1;
        static constexpr unsigned int dipswitches_lives_1 = 2;
        static constexpr unsigned int dipswitches_lives_2 = 3;
        static constexpr unsigned int dipswitches_bonus_life_1 = 4;
        static constexpr unsigned int dipswitches_bonus_life_2 = 5;
        static constexpr unsigned int dipswitches_difficulty = 6;
        static constexpr unsigned int dipswitches_ghost_names = 7;

        /*
         *  0x5000	1	Mirror, Read Port
                0xaf3f, IN0
                 0x0001	Joystick Up	Active Low
                 0x0002	Joystick Left	Active Low
                 0x0004	Joystick Right	Active Low
                 0x0008	Joystick Down	Active Low
                 0x0010	Off	Active High
                 0x0000	On	Active High
                 0x0020	Coin 1	Active Low
                 0x0040	Coin 2	Active Low
                 0x0080	Service 1	Active Low
            0x5001	1	Mirror, Device Write
                0xaf38, namco, namco_device, pacman_sound_enable_w
            0x5002	1	Mirror, Write NOP	0xaf38,
            0x5003	1	Mirror, Write
                0xaf38, pacman_flipscreen_w
            0x5004-0x5005	2	Mirror, Write NOP
                0xaf38, (// AM_WRITE(pacman_leds_w))
            0x5006	1	Mirror, Write NOP
                0xaf38, (// AM_WRITE(pacman_coin_lockout_global_w))
            0x5007	1	Mirror, Write
                0xaf38, pacman_coin_counter_w
            0x5040	1	Mirror, Read Port
                0xaf3f, IN1
                0x0001	Joystick Up	Active Low
                0x0002	Joystick Left	Active Low
                0x0004	Joystick Right	Active Low
                0x0008	Joystick Down	Active Low
                0x0020	Start 1	Active Low
                0x0040	Start 2	Active Low
                0x0080	Upright	Active High
                0x0000	Cocktail	Active High
            0x5040-0x505F	32	Mirror, Device Write	0xaf00, namco, namco_device, pacman_sound_w
            0x5060-0x506F	16	Mirror, Write Only, Shared	0xaf00, , spriteram2
            0x5070-0x507F	16	Mirror, Write NOP	0xaf00,
            0x5080	1	Mirror, Write NOP	0xaf3f,
            0x5080	1	Mirror, Read Port
                0xaf3f, DSW1
                0x0003	Coinage	Active High
                0x0003	2C_1C	Active High
                0x0001	1C_1C	Active High
                0x0002	1C_2C	Active High
                0x0000	Free_Play	Active High
                0x000c	Lives	Active High
                0x0000	1	Active High
                0x0004	2	Active High
                0x0008	3	Active High
                0x000c	5	Active High
                0x0030	Bonus_Life	Active High
                0x0000	10000	Active High
                0x0010	15000	Active High
                0x0020	20000	Active High
                0x0030	None	Active High
                0x0040	Difficulty	Active High
                0x0040	Normal	Active High
                0x0000	Hard	Active High
                0x0080	Normal	Active High
                0x0000	Alternate	Active High
            0x50C0	1	Mirror, Read Port
                0xaf3f, DSW2
                0x00ff	Unused	Active High
            0x50C0	1	Mirror, Device Write	0xaf3f, watchdog, watchdog_timer_device, reset_w
         */

        EmulatorMemory &m_memory;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_FOR_PACMAN_H
