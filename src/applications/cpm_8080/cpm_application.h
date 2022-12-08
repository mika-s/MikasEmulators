#pragma once

#include "chips/8080/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::misc {
class Session;
}

namespace emu::applications::cpm::i8080 {

using emu::i8080::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Emulator;
using emu::misc::Session;

class CpmApplication : public Emulator {
public:
    explicit CpmApplication(std::string const& file);

    std::unique_ptr<Session> new_session() override;

private:
    std::unique_ptr<Cpu> m_cpu;
    EmulatorMemory<u16, u8> m_memory;
    std::string m_loaded_file;

    void load_file(std::string const& file);

    static std::vector<u8> create_initial_offset();

    static std::vector<u8> create_work_ram(std::size_t size);

    static void patch_program(EmulatorMemory<u16, u8>& program);
};
}
