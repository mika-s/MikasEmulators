#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H

#include <memory>
#include "gui.h"
#include "settings.h"
#include "pacman_session.h"
#include "interfaces/input.h"
#include "chips/z80/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"

namespace emu::applications::pacman {

    using emu::misc::Emulator;

    class Pacman : public Emulator {
    public:
        Pacman(
                const Settings &settings,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input
        );

        std::unique_ptr<Session> new_session() override;

    private:
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
        static constexpr unsigned int board_test = 4;
        static constexpr unsigned int dipswitches_coinage_1 = 0;
        static constexpr unsigned int dipswitches_coinage_2 = 1;
        static constexpr unsigned int dipswitches_lives_1 = 2;
        static constexpr unsigned int dipswitches_lives_2 = 3;
        static constexpr unsigned int dipswitches_bonus_life_1 = 4;
        static constexpr unsigned int dipswitches_bonus_life_2 = 5;
        static constexpr unsigned int dipswitches_difficulty = 6;
        static constexpr unsigned int dipswitches_ghost_names = 7;

        EmulatorMemory m_memory;
        EmulatorMemory m_color_rom;
        EmulatorMemory m_palette_rom;
        EmulatorMemory m_tile_rom;
        EmulatorMemory m_sprite_rom;
        std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;

        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;

        void load_file();

        static std::vector<u8> create_tile_ram();

        static std::vector<u8> create_palette_ram();

        static std::vector<u8> create_ram();

        static std::vector<u8> create_sprite_ram();

        static std::vector<u8> create_memory_mapped_io();

        static std::vector<u8> fill_remaining(std::size_t memory_size);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H
