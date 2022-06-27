#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H

#include <cstdint>
#include <memory>
#include "8080/emulator_memory.h"
#include "8080/applications/space_invaders/gui.h"
#include "8080/applications/space_invaders/settings.h"
#include "8080/applications/space_invaders/space_invaders_session.h"
#include "8080/applications/space_invaders/interfaces/input.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080::applications::space_invaders {

    class SpaceInvaders : public Emulator8080 {
    public:
        SpaceInvaders(
                const Settings &settings,
                std::shared_ptr<Gui> gui,
                std::shared_ptr<Input> input
        );

        std::unique_ptr<Session> new_session() override;

    private:
        Settings m_settings;
        EmulatorMemory m_memory;
        std::shared_ptr<Gui> m_gui;
        std::shared_ptr<Input> m_input;

        void load_file();

        static std::vector<u8> create_work_ram();

        static std::vector<u8> create_vram();
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_SPACE_INVADERS_H
