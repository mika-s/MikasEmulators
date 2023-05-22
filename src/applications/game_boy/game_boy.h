#pragma once

#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include "game_boy_session.h"
#include <memory>

namespace emu::applications::game_boy {
class Audio;
class Gui;
class Input;
class MemoryMappedIoForGameBoy;
class Settings;
class Timer;
}
namespace emu::misc {
class Session;
}

namespace emu::applications::game_boy {

using emu::gui::GuiType;
using emu::misc::Emulator;

class GameBoy : public Emulator {
public:
    GameBoy(
        Settings const& settings,
        const GuiType gui_type);

    std::unique_ptr<Session> new_session() override;

private:
    EmulatorMemory<u16, u8> m_memory;
    EmulatorMemory<u16, u8> m_color_rom;
    EmulatorMemory<u16, u8> m_palette_rom;
    EmulatorMemory<u16, u8> m_tile_rom;
    EmulatorMemory<u16, u8> m_sprite_rom;
    EmulatorMemory<u16, u8> m_sound_rom1;
    EmulatorMemory<u16, u8> m_sound_rom2;
    std::shared_ptr<MemoryMappedIoForGameBoy> m_memory_mapped_io;

    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Audio> m_audio;
    bool m_is_starting_paused;

    void load_files();
};
}
