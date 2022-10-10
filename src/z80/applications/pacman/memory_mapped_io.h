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

        u8 read_in0();

        u8 read_in1();

        void write_in0(unsigned int bit_number, bool is_setting);

        void write_in1(unsigned int bit_number, bool is_setting);

        u8 read_coin_counter();

        void set_dipswitches(const Settings &settings);

        u8 read_dipswitches();

        void memory_changed(u16 address) override;

        void set_initial_values();

    private:
        static constexpr u16 from_address = 0x5000;
        static constexpr u16 to_address = 0x50ff;
        static constexpr u16 address_in0 = 0x5000;
        static constexpr u16 address_sound_enable = 0x5001;
        static constexpr u16 address_flip_screen = 0x5003;
        static constexpr u16 address_coin_counter = 0x5007;
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

        EmulatorMemory &m_memory;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_FOR_PACMAN_H
