#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H

#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "pacman_session.h"
#include <memory>
#include <vector>

namespace emu::applications::pacman {
    class Audio;
}
namespace emu::applications::pacman {
    class Gui;
}
namespace emu::applications::pacman {
    class Input;
}
namespace emu::applications::pacman {
    class MemoryMappedIoForPacman;
}
namespace emu::applications::pacman {
    class Settings;
}
namespace emu::misc {
    class Session;
}

namespace emu::applications::pacman {

    using emu::gui::GuiType;
    using emu::misc::Emulator;

    class Pacman : public Emulator {
    public:
        Pacman(
                const Settings &settings,
                const GuiType gui_type
        );

        std::unique_ptr<Session> new_session() override;

    private:
        EmulatorMemory m_memory;
        EmulatorMemory m_color_rom;
        EmulatorMemory m_palette_rom;
        EmulatorMemory m_tile_rom;
        EmulatorMemory m_sprite_rom;
        EmulatorMemory m_sound_rom1;
        EmulatorMemory m_sound_rom2;
        std::shared_ptr<MemoryMappedIoForPacman> m_memory_mapped_io;

        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        std::shared_ptr<Audio> m_audio;
        RunStatus m_startup_runstatus;

        void load_files();

        static std::vector<u8> create_tile_ram();

        static std::vector<u8> create_palette_ram();

        static std::vector<u8> create_ram();

        static std::vector<u8> create_sprite_ram();

        static std::vector<u8> create_memory_mapped_io();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_PACMAN_H
