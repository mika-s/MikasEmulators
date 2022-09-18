#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_H

#include <cstdint>
#include <memory>
#include "z80/emulator_memory.h"
#include "z80/applications/pacman/gui.h"
#include "z80/applications/pacman/settings.h"
#include "z80/applications/pacman/pacman_session.h"
#include "z80/applications/pacman/interfaces/input.h"
#include "z80/interfaces/emulatorZ80.h"
#include "crosscutting/typedefs.h"

namespace emu::z80::applications::pacman {

    class Pacman : public EmulatorZ80 {
    public:
        Pacman(
                const Settings &settings,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input
        );

        std::unique_ptr<Session> new_session() override;

    private:
        Settings m_settings;
        EmulatorMemory m_memory;
        EmulatorMemory m_tile_rom;
        EmulatorMemory m_sprite_rom;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;

        void load_file();

        static std::vector<u8> create_tile_ram();

        static std::vector<u8> create_palette_ram();

        static std::vector<u8> create_ram();

        static std::vector<u8> create_sprite_ram();

        static std::vector<u8> create_memory_mapped_io();
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_PACMAN_H
