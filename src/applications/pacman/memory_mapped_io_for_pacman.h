#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_MEMORY_MAPPED_IO_H

#include "settings.h"
#include "chips/z80/emulator_memory.h"
#include "chips/z80/interfaces/memory_mapped_io.h"
#include "crosscutting/typedefs.h"

namespace emu::applications::pacman {

    using emu::z80::EmulatorMemory;
    using emu::z80::MemoryMappedIo;

    class MemoryMappedIoForPacman : public MemoryMappedIo {
    public:
        explicit MemoryMappedIoForPacman(EmulatorMemory &memory);

        void write(u16 address, u8 value) override;

        u8 read(u16 address) override;

        bool is_interrupt_enabled();

        void in0_read(unsigned int bit_number, bool is_setting);

        [[nodiscard]] u8 in0_read() const;

        void in1_read(unsigned int bit_number, bool is_setting);

        [[nodiscard]] u8 in1_read() const;

        void in0_write(u8 value);

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

        static constexpr std::size_t address_mask = 0x7fff;
        static constexpr u16 address_rom_end = 0x3fff;
        static constexpr u16 address_ram_end = 0x4fff;
        static constexpr u16 address_in0_beginning = 0x5000;
        static constexpr u16 address_in0_end = 0x503f;
        static constexpr u16 address_sound_enable = 0x5001;
        static constexpr u16 address_aux_board = 0x5002;
        static constexpr u16 address_flip_screen = 0x5003;
        static constexpr u16 address_lamp1 = 0x5004;
        static constexpr u16 address_lamp2 = 0x5005;
        static constexpr u16 address_coin_lockout = 0x5006;
        static constexpr u16 address_coin_counter = 0x5007;
        static constexpr u16 address_in1_beginning = 0x5040;
        static constexpr u16 address_in1_end = 0x507f;
        static constexpr u16 address_dipswitches_beginning = 0x5080;
        static constexpr u16 address_dipswitches_end = 0x50bf;
        static constexpr u16 address_audio_beginning = 0x5040;
        static constexpr u16 address_audio_end = 0x505f;
        static constexpr u16 address_sprite_coords_beginning = 0x5060;
        static constexpr u16 address_sprite_coords_end = 0x506f;
        static constexpr u16 address_watchdog_beginning = 0x50c0;
        static constexpr u16 address_watchdog_end = 0x50ff;
        static constexpr u16 address_pacman_memory_end = 0x50ff;

        EmulatorMemory &m_memory;

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
