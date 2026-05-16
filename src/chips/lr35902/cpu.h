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

    [[nodiscard]] auto can_run_next_instruction() const -> bool;

    auto next_instruction() -> cyc;

    void reset_state();

    void start();

    void stop();

    void set_state_manually(const ManualState &new_state);

    auto memory() -> EmulatorMemory<u16, u8>&;

    [[nodiscard]] auto a() const -> u8;

    [[nodiscard]] auto b() const -> u8;

    [[nodiscard]] auto c() const -> u8;

    [[nodiscard]] auto d() const -> u8;

    [[nodiscard]] auto e() const -> u8;

    [[nodiscard]] auto h() const -> u8;

    [[nodiscard]] auto l() const -> u8;

    [[nodiscard]] auto f() const -> u8;

    [[nodiscard]] auto pc() const -> u16;

    [[nodiscard]] auto sp() const -> u16;

    [[nodiscard]] auto is_inta() const -> bool;

    [[nodiscard]] auto ime() const -> bool;

    [[nodiscard]] auto ie() const -> bool;

    void interrupt(u8 new_pc);

private:
    bool m_is_halted { false };

    bool m_ime { false }; // interrupt master enable
    bool m_ie { false };  // interrupt enabled register
    u8 m_pc_from_interruptor { 0 };

    EmulatorMemory<u16, u8>& m_memory;
    std::size_t m_memory_size;

    u8 m_opcode { 0 }; // TODO: real value
//    u16 m_sp { 0xffff };
    u16 m_sp { 0 };
    u16 m_pc;
//    u8 m_acc_reg { 0xff }; // TODO: real value
    u8 m_acc_reg { 0 };
    u8 m_b_reg { 0 };
    u8 m_c_reg { 0 };
    u8 m_d_reg { 0 };
    u8 m_e_reg { 0 };
    u8 m_h_reg { 0 };
    u8 m_l_reg { 0 };
    Flags m_flag_reg;

    void next_bits_instruction(u8 bits_opcode, cyc& cycles);

    auto handle_interrupt(cyc cycles) -> cyc;

    auto get_next_byte() -> NextByte;

    auto get_next_word() -> NextWord;

    [[nodiscard]] auto address_in_HL() const -> u16;

    void print_debug(u8 opcode) const;
};
}
