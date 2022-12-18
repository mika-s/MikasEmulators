#include "pacman.h"
#include "audio.h"
#include "crosscutting/util/file_util.h"
#include "gui.h"
#include "gui_imgui.h"
#include "gui_sdl.h"
#include "input_imgui.h"
#include "input_sdl.h"
#include "memory_mapped_io_for_pacman.h"
#include "pacman_session.h"
#include "settings.h" // IWYU pragma: keep
#include <cstddef>
#include <memory>
#include <string>

namespace emu::misc {
class Session;
}

namespace emu::applications::pacman {

using emu::util::file::read_file_into_vector;

Pacman::Pacman(Settings const& settings, const GuiType gui_type)
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

    load_files();

    m_memory_mapped_io = std::make_shared<MemoryMappedIoForPacman>(m_memory, settings);
    m_memory.attach_memory_mapper(m_memory_mapped_io);
}

std::unique_ptr<Session> Pacman::new_session()
{
    return std::make_unique<PacmanSession>(
        m_is_starting_paused,
        m_gui,
        m_input,
        m_audio,
        m_memory_mapped_io,
        m_memory);
}

void Pacman::load_files()
{
    const std::string directory = "roms/z80/pacman/";
    m_memory.add(read_file_into_vector(directory + "pacman.6e")); // $0000-$0fff: pacman.6e, code
    m_memory.add(read_file_into_vector(directory + "pacman.6f")); // $1000-$1fff: pacman.6f, code
    m_memory.add(read_file_into_vector(directory + "pacman.6h")); // $2000-$2fff: pacman.6h, code
    m_memory.add(read_file_into_vector(directory + "pacman.6j")); // $3000-$3fff: pacman.6j, code
    m_memory.add(create_tile_ram());                              // $4000-$43ff: tile RAM
    m_memory.add(create_palette_ram());                           // $4400-$47ff: palette RAM
    m_memory.add(create_ram());                                   // $4800-$4fef: RAM
    m_memory.add(create_sprite_ram());                            // $4ff0-$4fff: sprite RAM
    m_memory.add(create_memory_mapped_io());                      // $5000-$50ff: memory-mapped IO

    m_color_rom.add(read_file_into_vector(directory + "82s123.7f"));   // $0000-$0020: 82s123.7f, colors
    m_palette_rom.add(read_file_into_vector(directory + "82s126.4a")); // $0000-$00ff: 82s126.4a, palettes
    m_tile_rom.add(read_file_into_vector(directory + "pacman.5e"));    // $0000-$0fff: pacman.5e, tiles
    m_sprite_rom.add(read_file_into_vector(directory + "pacman.5f"));  // $0000-$0fff: pacman.5f, sprites
    m_sound_rom1.add(read_file_into_vector(directory + "82s126.1m"));  // $0000-$00ff: 82s126.1m, sound 1
    m_sound_rom2.add(read_file_into_vector(directory + "82s126.3m"));  // $0000-$00ff: 82s126.3m, sound 2

    m_gui->load_color_rom({ m_color_rom.begin(), m_color_rom.end() });
    m_gui->load_palette_rom({ m_palette_rom.begin(), m_palette_rom.end() });
    m_gui->load_tile_rom({ m_tile_rom.begin(), m_tile_rom.end() });
    m_gui->load_sprite_rom({ m_sprite_rom.begin(), m_sprite_rom.end() });

    std::vector<u8> sound_rom1 = { m_sound_rom1.begin(), m_sound_rom1.end() };
    std::vector<u8> sound_rom2 = { m_sound_rom2.begin(), m_sound_rom2.end() };
    m_audio = std::make_shared<Audio>(sound_rom1, sound_rom2);
}

std::vector<u8> create_empty_vector(std::size_t size)
{
    std::vector<u8> vec(size, 0);
    return vec;
}

std::vector<u8> Pacman::create_tile_ram()
{
    return create_empty_vector(0x43ff - 0x4000 + 1);
}

std::vector<u8> Pacman::create_palette_ram()
{
    return create_empty_vector(0x47ff - 0x4400 + 1);
}

std::vector<u8> Pacman::create_ram()
{
    return create_empty_vector(0x4fef - 0x4800 + 1);
}

std::vector<u8> Pacman::create_sprite_ram()
{
    return create_empty_vector(0x4fff - 0x4ff0 + 1);
}

std::vector<u8> Pacman::create_memory_mapped_io()
{
    return create_empty_vector(0x50ff - 0x5000 + 1);
}
}
