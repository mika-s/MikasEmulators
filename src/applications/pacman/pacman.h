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

        Settings m_settings;

        EmulatorMemory m_memory;
        EmulatorMemory m_color_rom;
        EmulatorMemory m_palette_rom;
        EmulatorMemory m_tile_rom;
        EmulatorMemory m_sprite_rom;
        std::shared_ptr<MemoryMappedIo> m_memory_mapped_io;

        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;

        void load_file();

        static std::vector<u8> create_tile_ram();

        static std::vector<u8> create_palette_ram();

        static std::vector<u8> create_ram();

        static std::vector<u8> create_sprite_ram();

        static std::vector<u8> create_memory_mapped_io();

        static std::vector<u8> fill_remaining(size_t memory_size);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H
