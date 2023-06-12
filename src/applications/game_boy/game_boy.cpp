#include "game_boy.h"
#include "crosscutting/util/file_util.h"
#include "game_boy_session.h"
#include "gui.h"
#include "gui_imgui.h"
#include "gui_sdl.h"
#include "input_imgui.h"
#include "input_sdl.h"
#include "lcd.h"
#include "memory_mapped_io_for_game_boy.h"
#include "settings.h" // IWYU pragma: keep
#include "timer.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::game_boy {

using emu::util::file::read_file_into_vector;

GameBoy::GameBoy(Settings const& settings, const GuiType gui_type)
{
    if (gui_type == GuiType::DEBUGGING) {
        m_gui = std::make_shared<GuiImgui>();
        m_input = std::make_shared<InputImgui>();
        m_is_starting_paused = true;
    } else {
        m_gui = std::make_shared<GuiSdl>();
        m_input = std::make_shared<InputSdl>();
        m_is_starting_paused = false;
    }

    m_lcd = std::make_shared<Lcd>();
    m_timer = std::make_shared<Timer>();

    load_files();

    m_memory_mapped_io = std::make_shared<MemoryMappedIoForGameBoy>(m_memory, m_timer, m_lcd, settings);
    m_memory.attach_memory_mapper(m_memory_mapped_io);
    m_gui->attach_memory_mapper(m_memory_mapped_io);
}

std::unique_ptr<Session> GameBoy::new_session()
{
    return std::make_unique<GameBoySession>(
        m_is_starting_paused,
        m_gui,
        m_lcd,
        m_input,
        m_audio,
        m_timer,
        m_memory_mapped_io,
        m_memory);
}

std::vector<u8> create_empty_vector(std::size_t size)
{
    std::vector<u8> vec(size, 0);
    return vec;
}

void GameBoy::load_files()
{
    const std::string directory = "roms/game_boy/";
    m_memory.add(read_file_into_vector(directory + "06.gb")); // hardcoded tests
    m_memory.add(create_empty_vector(0xffff - m_memory.size()));

    // [0000-3FFF] Cartridge ROM, bank 0
    //      [0000-00FF] BIOS
    //      [0100-014F] Cartridge header
    // [4000-7FFF] Cartridge ROM, other banks
    // [8000-9FFF] Graphics RAM
    // [A000-BFFF] Cartridge (External) RAM
    // [C000-DFFF] Working RAM
    // [E000-FDFF] Working RAM (shadow)
    // [FE00-FE9F] Graphics: sprite information
    // [FF00-FF7F] Memory-mapped I/O
    // [FF80-FFFF] Zero-page RAM

    //    m_gui->load_color_rom({ m_color_rom.begin(), m_color_rom.end() });
    //    m_gui->load_palette_rom({ m_palette_rom.begin(), m_palette_rom.end() });
    //    m_gui->load_tile_rom({ m_tile_rom.begin(), m_tile_rom.end() });
    //    m_gui->load_sprite_rom({ m_sprite_rom.begin(), m_sprite_rom.end() });

    //    std::vector<u8> sound_rom1 = { m_sound_rom1.begin(), m_sound_rom1.end() };
    //    std::vector<u8> sound_rom2 = { m_sound_rom2.begin(), m_sound_rom2.end() };
    //    m_audio = std::make_shared<Audio>(sound_rom1, sound_rom2);
}
}
