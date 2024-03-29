#include "zxspectrum_48k.h"
#include "crosscutting/util/file_util.h"
#include "formats/z80_format.h"
#include "gui.h"
#include "gui_imgui.h"
#include "gui_sdl.h"
#include "input_imgui.h"
#include "input_sdl.h"
#include "interfaces/format.h"
#include "memory_map_for_zxspectrum_48k.h"
#include "settings.h"
#include "zxspectrum_48k_print_header_session.h"
#include "zxspectrum_48k_session.h"
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::zxspectrum_48k {

using emu::util::file::read_file_into_vector;

ZxSpectrum48k::ZxSpectrum48k(Settings settings, const GuiType gui_type)
    : m_settings(std::move(settings))
{
    if (m_settings.m_is_only_printing_header) {
        setup_printing_session();
    } else {
        setup_ordinary_session(gui_type);
    }
}

void ZxSpectrum48k::setup_printing_session()
{
    m_format = std::make_shared<Z80Format>(m_settings.m_snapshot_file);
}

void ZxSpectrum48k::setup_ordinary_session(const GuiType gui_type)
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

    if (!m_settings.m_snapshot_file.empty()) {
        load_snapshot();
    }

    m_memory_mapped_io = std::make_shared<MemoryMapForZxSpectrum48k>(m_memory);
    m_memory.attach_memory_mapper(m_memory_mapped_io);
    m_gui->create_table();
}

std::unique_ptr<Session> ZxSpectrum48k::new_session()
{
    if (m_settings.m_is_only_printing_header) {
        return std::make_unique<ZxSpectrum48kPrintHeaderSession>(m_format);
    } else if (!m_settings.m_snapshot_file.empty()) {
        return std::make_unique<ZxSpectrum48kSession>(m_settings, m_is_starting_paused, m_gui, m_input, m_memory, m_format->to_cpu_state());
    } else {
        return std::make_unique<ZxSpectrum48kSession>(m_settings, m_is_starting_paused, m_gui, m_input, m_memory);
    }
}

std::vector<u8> create_empty_vector(std::size_t size)
{
    std::vector<u8> vec(size, 0);
    return vec;
}

void ZxSpectrum48k::load_files()
{
    const std::string directory = "roms/z80/zxspectrum_48k/";
    m_memory.add(read_file_into_vector(directory + "48k.rom")); // $0000-$3fff: 48k.rom
    m_memory.add(create_empty_vector(0x57ff - 0x4000 + 1));     // $4000-$57ff: video RAM
    m_memory.add(create_empty_vector(0x5aff - 0x5800 + 1));     // $5800-$5aff: video RAM (color data)
    m_memory.add(create_empty_vector(0x5bff - 0x5b00 + 1));     // $5b00-$5bff: printer buffer
    m_memory.add(create_empty_vector(0x5cbf - 0x5c00 + 1));     // $5c00-$5cbf: system variables
    m_memory.add(create_empty_vector(0x5cca - 0x5cc0 + 1));     // $5cc0-$5cca: reserved
    m_memory.add(create_empty_vector(0xff57 - 0x5ccb + 1));     // $5ccb-$ff57: RAM
    m_memory.add(create_empty_vector(0xffff - 0xff58 + 1));     // $ff58-$ffff: reserved
}

void ZxSpectrum48k::load_snapshot()
{

    m_format = std::make_shared<Z80Format>(m_settings.m_snapshot_file);
    m_format->to_memory(m_memory);
}
}
