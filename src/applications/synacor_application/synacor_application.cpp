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
#include <iosfwd>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::synacor {

using emu::gui::GuiType;
using emu::synacor::Data;
using emu::util::byte::to_u16;
using emu::util::file::read_file;
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

    const std::stringstream file_content = read_file(m_loaded_file);
    m_file_content = file_content.str();
    std::vector<Data> remaining_memory;

//    if (code.size() < s_memory_size) {
//        remaining_memory = create_work_ram(s_memory_size - code.size());
//    }

//    assert(code.size() + remaining_memory.size() == s_memory_size);

//    m_memory.add(code);
    m_memory.add(remaining_memory);
}

std::vector<Data> SynacorApplication::create_work_ram(std::size_t size)
{
    std::vector<Data> work_ram;

    work_ram.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        work_ram.emplace_back(0);
    }

    return work_ram;
}
}
