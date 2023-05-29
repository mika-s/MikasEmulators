#pragma once

#include "crosscutting/typedefs.h"
#include "flags.h"

namespace emu::lr35902 {

struct ManualState {
    bool m_ime;
    bool m_ie;
    u16 m_sp;
    u16 m_pc;
    u8 m_acc_reg;
    u8 m_b_reg;
    u8 m_c_reg;
    u8 m_d_reg;
    u8 m_e_reg;
    u8 m_h_reg;
    u8 m_l_reg;
    Flags m_flag_reg;
};

}
