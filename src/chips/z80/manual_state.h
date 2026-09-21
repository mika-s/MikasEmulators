#pragma once

#include "crosscutting/typedefs.h"
#include "flags.h"
#include "interrupt_mode.h"

namespace emu::z80 {

struct ManualState {
    bool m_iff1 { false };
    bool m_iff2 { false };
    u16 m_sp { 0 };
    u16 m_pc { 0 };
    u8 m_acc_reg { 0 };
    u8 m_acc_p_reg { 0 };
    u8 m_b_reg { 0 };
    u8 m_b_p_reg { 0 };
    u8 m_c_reg { 0 };
    u8 m_c_p_reg { 0 };
    u8 m_d_reg { 0 };
    u8 m_d_p_reg { 0 };
    u8 m_e_reg { 0 };
    u8 m_e_p_reg { 0 };
    u8 m_h_reg { 0 };
    u8 m_h_p_reg { 0 };
    u8 m_l_reg { 0 };
    u8 m_l_p_reg { 0 };
    u16 m_ix_reg { 0 };
    u16 m_iy_reg { 0 };
    u8 m_i_reg { 0 };
    u8 m_r_reg { 0 };
    Flags m_flag_reg;
    Flags m_flag_p_reg;
    InterruptMode m_interrupt_mode { InterruptMode::ZERO };
};

}
