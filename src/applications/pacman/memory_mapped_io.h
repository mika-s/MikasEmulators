#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H

#include "settings.h"
#include "chips/z80/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::applications::pacman {

    using emu::z80::EmulatorMemory;

    class MemoryMappedIo {
    public:
        explicit MemoryMappedIo(EmulatorMemory &memory);

        bool is_interrupt_enabled();

        bool read_in0(unsigned int bit_number);

        u8 read_in0() const;

        u8 read_in1() const;

        void write_in0(unsigned int bit_number, bool is_setting);

        void write_in1(unsigned int bit_number, bool is_setting);

        u8 read_coin_counter();

        void set_dipswitches(const Settings &settings);

        u8 read_dipswitches();

        void set_board_test(const Settings &settings);

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
        static constexpr unsigned int board_test = 4;
        static constexpr unsigned int dipswitches_coinage_1 = 0;
        static constexpr unsigned int dipswitches_coinage_2 = 1;
        static constexpr unsigned int dipswitches_lives_1 = 2;
        static constexpr unsigned int dipswitches_lives_2 = 3;
        static constexpr unsigned int dipswitches_bonus_life_1 = 4;
        static constexpr unsigned int dipswitches_bonus_life_2 = 5;
        static constexpr unsigned int dipswitches_difficulty = 6;
        static constexpr unsigned int dipswitches_ghost_names = 7;

        EmulatorMemory &m_memory;

        u8 m_initial_value_in0 = 0b10011111;
        u8 m_initial_value_in1 = 0b11111111;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H
