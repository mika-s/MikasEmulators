#pragma once

#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include "settings.h"
#include "space_invaders_session.h"
#include <memory>

namespace emu::applications::space_invaders {
class Gui;
class Input;
class MemoryMapForSpaceInvaders;
}
namespace emu::misc {
class Session;
}

namespace emu::applications::space_invaders {

using emu::gui::GuiType;
using emu::misc::Emulator;

class SpaceInvaders : public Emulator {
public:
    SpaceInvaders(
        Settings const& settings,
        const GuiType gui_type);

    std::unique_ptr<Session> new_session() override;

private:
    Settings m_settings;
    EmulatorMemory<u16, u8> m_memory;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    bool m_is_starting_paused;
    std::shared_ptr<MemoryMapForSpaceInvaders> m_memory_mapped_io;

    void load_files();
};
}
