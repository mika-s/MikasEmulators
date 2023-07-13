#include "synacor_application.h"
#include "chips/trivial/synacor/usings.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include "gui_imgui.h"
#include "input_imgui.h"
#include "synacor_application_session.h"
#include "tui_terminal.h"
#include <cassert>
#include <fmt/core.h>
#include <iosfwd>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::synacor {

using emu::gui::GuiType;
using emu::synacor::Data;
using emu::util::byte::to_u16;
using emu::util::file::read_file_into_vector;
using emu::util::string::split;

SynacorApplication::SynacorApplication(const GuiType gui_type)
{
    load_file();
    if (gui_type == GuiType::DEBUGGING) {
        m_is_only_run_once = false;
        m_gui = std::make_shared<GuiImgui>();
        m_input = std::make_shared<InputImgui>();
        m_is_starting_paused = true;
    } else {
        m_is_only_run_once = true;
        m_gui = std::make_shared<TuiTerminal>();
        m_input = std::make_shared<InputImgui>();
        m_is_starting_paused = false;
    }

    m_memory_mapped_io = std::make_shared<MemoryMapForSynacorApplication>(m_memory);
    m_memory.attach_memory_mapper(m_memory_mapped_io);
}

std::unique_ptr<Session> SynacorApplication::new_session()
{
    return std::make_unique<SynacorApplicationSession>(
        m_is_only_run_once,
        m_is_starting_paused,
        m_gui,
        m_input,
        m_loaded_file,
        m_file_content,
        m_memory);
}

void SynacorApplication::load_file()
{
    m_loaded_file = "roms/trivial/synacor/challenge.bin";
    std::vector<u8> as_u8 = read_file_into_vector(m_loaded_file);
    std::vector<u16> as_u16;
    for (unsigned int i = 0; i < as_u8.size(); i += 2) {
        as_u16.push_back(to_u16(as_u8[i + 1], as_u8[i]));
    }

    std::vector<RawData> as_RawData;
    for (u16 value : as_u16) {
        if (value >= s_max_value_in_file) {
            throw std::runtime_error(
                fmt::format("Value too large in {}. Max value is {}, but was {}", m_loaded_file, s_max_value_in_file, value));
        } else {
            as_RawData.emplace_back(value);
        }
    }

    m_memory.add(as_RawData);
}

}
