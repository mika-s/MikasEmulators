#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H

#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include "interfaces/input.h"
#include "memory_map_for_space_invaders.h"
#include "settings.h"
#include "space_invaders_session.h"
#include <memory>

namespace emu::applications::space_invaders {

    using emu::gui::GuiType;
    using emu::misc::Emulator;
    using emu::misc::RunStatus;

    class SpaceInvaders : public Emulator {
    public:
        SpaceInvaders(
                const Settings &settings,
                const GuiType gui_type
        );

        std::unique_ptr<Session> new_session() override;

    private:
        Settings m_settings;
        EmulatorMemory m_memory;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;
        RunStatus m_startup_runstatus;
        std::shared_ptr<MemoryMapForSpaceInvaders> m_memory_mapped_io;

        void load_files();

        static std::vector<u8> create_work_ram();

        static std::vector<u8> create_vram();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
