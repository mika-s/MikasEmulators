#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H

#include "settings.h"
#include "chips/z80/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::applications::pacman {

    using emu::z80::EmulatorMemory;

    class MemoryMappedIo {
    public:
        bool is_interrupt_enabled();

        void in0_read(unsigned int bit_number, bool is_setting);

        bool in0_read(unsigned int bit_number);

        void in1_read(unsigned int bit_number, bool is_setting);

        bool in1_read(unsigned int bit_number);

        u8 in0_write() const;

        u8 in0_read() const;

        u8 in1_write() const;

        u8 in1_read() const;

        void in0_write(u8 value);

        void in0_write(unsigned int bit_number, bool is_setting);

        void in1_write(u8 value);

        void in1_write(unsigned int bit_number, bool is_setting);

        u8 coin_counter();

        u8 flip_screen();

        void flip_screen(u8 value);

        void dipswitches(const Settings &settings);

        u8 dipswitches();

        void board_test(const Settings &settings);

        bool is_sound_enabled();

        void is_sound_enabled(bool new_value);

        bool is_aux_board_enabled();

        void is_aux_board_enabled(bool new_value);

    private:
        static constexpr unsigned int board_test_bit = 4;
        static constexpr unsigned int dipswitches_coinage_1 = 0;
        static constexpr unsigned int dipswitches_coinage_2 = 1;
        static constexpr unsigned int dipswitches_lives_1 = 2;
        static constexpr unsigned int dipswitches_lives_2 = 3;
        static constexpr unsigned int dipswitches_bonus_life_1 = 4;
        static constexpr unsigned int dipswitches_bonus_life_2 = 5;
        static constexpr unsigned int dipswitches_difficulty = 6;
        static constexpr unsigned int dipswitches_ghost_names = 7;
        static constexpr u8 m_initial_value_in0_read = 0b10011111;
        static constexpr u8 m_initial_value_in1_read = 0b11111111;

        u8 m_in0_read = m_initial_value_in0_read;
        u8 m_in0_write;

        u8 m_in1_read = m_initial_value_in1_read;
        u8 m_in1_write;

        u8 m_dipswitches;

        bool m_is_sound_enabled;

        bool m_is_aux_board_enabled;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H
