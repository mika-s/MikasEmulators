#pragma once

#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "flags.h"
#include <cstddef>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::lr35902 {
struct ManualState;
}

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;

class Cpu {
public:
    Cpu(
        EmulatorMemory<u16, u8>& memory,
        u16 initial_pc);

    ~Cpu();

    [[nodiscard]] bool can_run_next_instruction() const;

    cyc next_instruction();

    void reset_state();

    void start();

    void stop();

    void set_state_manually(ManualState new_state);

    EmulatorMemory<u16, u8>& memory();

    [[nodiscard]] u8 a() const;

    [[nodiscard]] u8 b() const;

    [[nodiscard]] u8 c() const;

    [[nodiscard]] u8 d() const;

    [[nodiscard]] u8 e() const;

    [[nodiscard]] u8 h() const;

    [[nodiscard]] u8 l() const;

    [[nodiscard]] u8 f() const;

    [[nodiscard]] u16 pc() const;

    [[nodiscard]] u16 sp() const;

    [[nodiscard]] bool is_inta() const;

    [[nodiscard]] bool is_interrupted() const;

    [[nodiscard]] bool if_() const;

    [[nodiscard]] bool ie() const;

    void interrupt(u8 supplied_instruction_from_interruptor);

private:
    bool m_is_halted { false };

    bool m_if { false };
    bool m_ie { false };
    bool m_ime { false };
    bool m_is_interrupted { false };
    u8 m_instruction_from_interruptor { 0 };

    EmulatorMemory<u16, u8>& m_memory;
    std::size_t m_memory_size;

    u8 m_opcode { 0 };
    u16 m_sp { 0xffff };
    u16 m_pc;
    u8 m_acc_reg { 0xff };
    u8 m_b_reg { 0 };
    u8 m_c_reg { 0 };
    u8 m_d_reg { 0 };
    u8 m_e_reg { 0 };
    u8 m_h_reg { 0 };
    u8 m_l_reg { 0 };
    Flags m_flag_reg;

    void next_bits_instruction(u8 bits_opcode, cyc& cycles);

    cyc handle_maskable_interrupt_0(cyc cycles);

    NextByte get_next_byte();

    NextWord get_next_word();

    [[nodiscard]] u16 address_in_HL() const;

    void print_debug(u8 opcode);
};
}
