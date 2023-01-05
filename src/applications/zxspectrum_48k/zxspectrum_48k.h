#pragma once

#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include "settings.h"
#include "zxspectrum_48k_session.h"
#include <memory>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class Format;
class Gui;
class Input;
class MemoryMapForZxSpectrum48k;
}
namespace emu::misc {
class Session;
}

namespace emu::applications::zxspectrum_48k {

using emu::gui::GuiType;
using emu::misc::Emulator;

class ZxSpectrum48k : public Emulator {
public:
    ZxSpectrum48k(
        Settings settings,
        const GuiType gui_type);

    std::unique_ptr<Session> new_session() override;

private:
    Settings m_settings;
    EmulatorMemory<u16, u8> m_memory;
    std::shared_ptr<Gui> m_gui;
    std::shared_ptr<Input> m_input;
    bool m_is_starting_paused;
    std::shared_ptr<MemoryMapForZxSpectrum48k> m_memory_mapped_io;
    std::shared_ptr<Format> m_format;

    void setup_printing_session();

    void setup_ordinary_session(const GuiType gui_type);

    void load_files();

    void load_snapshot();

    static std::vector<u8> create_vram();

    static std::vector<u8> create_vram_color();

    static std::vector<u8> create_printer_buffer();

    static std::vector<u8> create_system_variables();

    static std::vector<u8> create_reserved1();

    static std::vector<u8> create_ram();

    static std::vector<u8> create_reserved2();
};
}
