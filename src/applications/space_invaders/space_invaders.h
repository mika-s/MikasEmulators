#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H

#include <memory>
#include "gui.h"
#include "settings.h"
#include "space_invaders_session.h"
#include "interfaces/input.h"
#include "chips/8080/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"

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

        void load_file();

        static std::vector<u8> create_work_ram();

        static std::vector<u8> create_vram();

        std::vector<u8> fill_remaining(std::size_t size);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
