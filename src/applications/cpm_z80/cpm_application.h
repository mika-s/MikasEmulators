#pragma once

#include "chips/z80/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::misc {
class Session;
}

namespace emu::applications::cpm::z80 {

using emu::memory::EmulatorMemory;
using emu::misc::Emulator;
using emu::misc::Session;
using emu::z80::Cpu;

class CpmApplication : public Emulator {
public:
    explicit CpmApplication(std::string const& file);

    std::unique_ptr<Session> new_session() override;

private:
    std::unique_ptr<Cpu> m_cpu;
    EmulatorMemory<u16, u8> m_memory;
    std::string m_loaded_file;

    void load_file(std::string const& file);

    static void patch_program(EmulatorMemory<u16, u8>& program);
};
}
