#pragma once

#include "crosscutting/typedefs.h"
#include "flags.h"
#include "interrupt_mode.h"

namespace emu::lr35902 {

struct ManualState {
    bool m_iff1;
    bool m_iff2;
    u16 m_sp;
    u16 m_pc;
    u8 m_acc_reg;
    u8 m_acc_p_reg;
    u8 m_b_reg;
    u8 m_b_p_reg;
    u8 m_c_reg;
    u8 m_c_p_reg;
    u8 m_d_reg;
    u8 m_d_p_reg;
    u8 m_e_reg;
    u8 m_e_p_reg;
    u8 m_h_reg;
    u8 m_h_p_reg;
    u8 m_l_reg;
    u8 m_l_p_reg;
    u16 m_ix_reg;
    u16 m_iy_reg;
    u8 m_i_reg;
    u8 m_r_reg;
    Flags m_flag_reg;
    Flags m_flag_p_reg;
    InterruptMode m_interrupt_mode;
};

}
