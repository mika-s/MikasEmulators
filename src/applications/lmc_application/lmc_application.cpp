#include "lmc_application.h"
#include "chips/trivial/lmc/assembler/assembler.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include "gui_imgui.h"
#include "input_imgui.h"
#include "lmc_application_session.h"
#include "tui_terminal.h"
#include <cassert>
#include <iosfwd>
#include <vector>

namespace emu::misc {
    class Session;
}

namespace emu::applications::lmc {

    using emu::gui::GuiType;
    using emu::lmc::Assembler;
    using emu::lmc::Data;
    using emu::util::byte::to_u16;
    using emu::util::file::read_file;
    using emu::util::string::split;

    LmcApplication::LmcApplication(const std::string &file, const GuiType gui_type) {
        load_file(file);
        if (gui_type == GuiType::DEBUGGING) {
            m_gui = std::make_shared<GuiImgui>();
            m_input = std::make_shared<InputImgui>();
            m_startup_runstatus = RunStatus::PAUSED;
        } else {
            m_gui = std::make_shared<TuiTerminal>();
            m_input = std::make_shared<InputImgui>();
            m_startup_runstatus = RunStatus::RUNNING;
        }
    }

    std::unique_ptr<Session> LmcApplication::new_session() {
        return std::make_unique<LmcApplicationSession>(m_startup_runstatus, m_gui, m_input, m_loaded_file, m_memory);
    }

    void LmcApplication::load_file(const std::string &file) {
        m_loaded_file = file;

        const std::stringstream file_content = read_file(file);
        const std::vector<Data> code = Assembler::assemble(file_content);
        std::vector<Data> remaining_memory;

        if (code.size() < memory_size) {
            remaining_memory = create_work_ram(memory_size - code.size());
        }

        assert(code.size() + remaining_memory.size() == memory_size);

        m_memory.add(code);
        m_memory.add(remaining_memory);
    }

    std::vector<Data> LmcApplication::create_work_ram(std::size_t size) {
        std::vector<Data> work_ram;

        work_ram.reserve(size);
        for (std::size_t i = 0; i < size; ++i) {
            work_ram.emplace_back(0);
        }

        return work_ram;
    }
}
