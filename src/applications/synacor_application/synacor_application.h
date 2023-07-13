#pragma once

#include "chips/trivial/synacor/cpu.h"
#include "chips/trivial/synacor/usings.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/uinteger.h"
#include "memory_map_for_synacor_application.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::synacor {
class Input;
class Ui;
}
namespace emu::misc {
class Session;
}

namespace emu::applications::synacor {

using emu::gui::GuiType;
using emu::memory::EmulatorMemory;
using emu::misc::Emulator;
using emu::misc::Session;
using emu::misc::UInteger;
using emu::synacor::Address;
using emu::synacor::Cpu;
using emu::synacor::Data;
using emu::synacor::RawData;

class SynacorApplication : public Emulator {
public:
    explicit SynacorApplication(GuiType gui_type);

    std::unique_ptr<Session> new_session() override;

private:
    static constexpr u16 s_max_value_in_file = 32776;

    bool m_is_only_run_once;
    std::unique_ptr<Cpu> m_cpu;
    EmulatorMemory<Address, RawData> m_memory;
    std::shared_ptr<Ui> m_gui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<MemoryMapForSynacorApplication> m_memory_mapped_io;
    bool m_is_starting_paused;
    std::string m_loaded_file;
    std::string m_file_content;

    void load_file();
};
}
