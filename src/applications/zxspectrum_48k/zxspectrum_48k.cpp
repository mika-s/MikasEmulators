#include "zxspectrum_48k.h"
#include "crosscutting/util/file_util.h"
#include "gui_imgui.h"
#include "gui_sdl.h"
#include "input_imgui.h"
#include "input_sdl.h"
#include "memory_map_for_zxspectrum_48k.h"
#include "zxspectrum_48k_session.h"
#include <cstddef>
#include <string>

namespace emu::applications::zxspectrum_48k {
class Settings;
}
namespace emu::misc {
class Session;
}

namespace emu::applications::zxspectrum_48k {

using emu::util::file::read_file_into_vector;

ZxSpectrum48k::ZxSpectrum48k(Settings const& settings, const GuiType gui_type)
    : m_settings(settings)
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

    m_memory_mapped_io = std::make_shared<MemoryMapForZxSpectrum48k>(m_memory);
    m_memory.attach_memory_mapper(m_memory_mapped_io);
}

std::unique_ptr<Session> ZxSpectrum48k::new_session()
{
    return std::make_unique<ZxSpectrum48kSession>(m_settings, m_is_starting_paused, m_gui, m_input, m_memory);
}

void ZxSpectrum48k::load_files()
{
    const std::string directory = "roms/z80/zxspectrum_48k/";
    m_memory.add(read_file_into_vector(directory + "48k.rom")); // $0000-$3fff: 48k.rom
    m_memory.add(create_vram());                                // $4000-$57ff: video RAM
    m_memory.add(create_vram_color());                          // $5800-$5aff: video RAM (color data)
    m_memory.add(create_printer_buffer());                      // $5b00-$5bff: printer buffer
    m_memory.add(create_system_variables());                    // $5c00-$5cbf: system variables
    m_memory.add(create_reserved1());                           // $5cc0-$5cca: reserved
    m_memory.add(create_ram());                                 // $5ccb-$ff57: RAM
    m_memory.add(create_reserved2());                           // $ff58-$ffff: reserved
}

std::vector<u8> create_empty_vector(std::size_t size)
{
    std::vector<u8> vec(size, 0);
    return vec;
}

std::vector<u8> ZxSpectrum48k::create_vram()
{
    return create_empty_vector(0x57ff - 0x4000 + 1);
}

std::vector<u8> ZxSpectrum48k::create_vram_color()
{
    return create_empty_vector(0x5aff - 0x5800 + 1);
}

std::vector<u8> ZxSpectrum48k::create_printer_buffer()
{
    return create_empty_vector(0x5bff - 0x5b00 + 1);
}

std::vector<u8> ZxSpectrum48k::create_system_variables()
{
    return create_empty_vector(0x5cbf - 0x5c00 + 1);
}

std::vector<u8> ZxSpectrum48k::create_reserved1()
{
    return create_empty_vector(0x5cca - 0x5cc0 + 1);
}

std::vector<u8> ZxSpectrum48k::create_ram()
{
    return create_empty_vector(0xff57 - 0x5ccb + 1);
}

std::vector<u8> ZxSpectrum48k::create_reserved2()
{
    return create_empty_vector(0xffff - 0xff58 + 1);
}
}
