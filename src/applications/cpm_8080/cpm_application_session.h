#pragma once

#include "chips/8080/cpu.h"
#include "chips/8080/interfaces/out_observer.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::applications::cpm::i8080 {

using emu::i8080::Cpu;
using emu::i8080::OutObserver;
using emu::memory::EmulatorMemory;
using emu::misc::Session;

class CpmApplicationSession
    : public Session
    , public OutObserver {
public:
    CpmApplicationSession(
        std::string loaded_file,
        EmulatorMemory<u16, u8> m_memory);

    void run() override;

    void pause() override;

    void stop() override;

    void out_changed(u8 port) override;

private:
    static constexpr u8 s_finished_port = 0;
    static constexpr u8 s_output_port = 1;
    static constexpr u8 s_C_WRITE = 2;
    static constexpr u8 s_C_WRITESTR = 9;

    std::unique_ptr<Cpu> m_cpu;
    EmulatorMemory<u16, u8> m_memory;
    std::string m_loaded_file;
    bool m_is_finished;

    void setup_cpu();

    // CP/M syscalls
    // https://www.seasip.info/Cpm/bdos.html

    static void c_write(u8 e);

    static void c_writestr(EmulatorMemory<u16, u8> const& memory, u16 address);
};
}
