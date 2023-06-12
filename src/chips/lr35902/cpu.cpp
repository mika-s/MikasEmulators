#include "cpu.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "manual_state.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::string::hexify;

Cpu::Cpu(EmulatorMemory<u16, u8>& memory, const u16 initial_pc)
    : m_memory(memory)
    , m_memory_size(memory.size())
    , m_pc(initial_pc)
{
    //    m_flag_reg.from_u8(0xff);  // TODO: real value
    m_flag_reg.from_u8(0x0);
}

Cpu::~Cpu() = default;

bool Cpu::can_run_next_instruction() const
{
    return m_pc < m_memory_size;
}

void Cpu::reset_state()
{
    m_acc_reg = 0xff;
    m_b_reg = 0;
    m_c_reg = 0;
    m_d_reg = 0;
    m_e_reg = 0;
    m_h_reg = 0;
    m_l_reg = 0;
    m_flag_reg.from_u8(0xff);
    m_pc = 0x100;
    m_sp = 0xfffe;
    m_is_halted = false;
    m_ime = m_ie = false;
    m_pc_from_interruptor = 0;
}

void Cpu::start()
{
}

void Cpu::stop()
{
    reset_state();
}

void Cpu::set_state_manually(ManualState manual_state)
{
    m_ime = manual_state.m_ime;
    m_ie = manual_state.m_ie;
    m_sp = manual_state.m_sp;
    m_pc = manual_state.m_pc;
    m_acc_reg = manual_state.m_acc_reg;
    m_b_reg = manual_state.m_b_reg;
    m_c_reg = manual_state.m_c_reg;
    m_d_reg = manual_state.m_d_reg;
    m_e_reg = manual_state.m_e_reg;
    m_h_reg = manual_state.m_h_reg;
    m_l_reg = manual_state.m_l_reg;
    m_flag_reg.from_u8(manual_state.m_flag_reg.to_u8());
}

void Cpu::interrupt(u8 new_pc)
{
    m_ie = true;
    m_pc_from_interruptor = new_pc;
}

bool Cpu::is_inta() const
{
    return m_ime;
}

cyc Cpu::next_instruction()
{
    cyc cycles = 0;

    if (m_ime && m_ie) {
        cycles += handle_interrupt(cycles);
    } else if (m_is_halted) {
        return 4; // TODO: What is the proper value while NOPing during halt?
    } else {
        m_opcode = get_next_byte().farg;
    }

    print_debug(m_opcode);

    switch (m_opcode) {
    case NOP:
        nop(cycles);
        break;
    case LD_BC_nn:
        ld_dd_nn(m_b_reg, m_c_reg, get_next_word(), cycles);
        break;
    case LD_MBC_A:
        ld_Mss_A(m_memory, to_u16(m_b_reg, m_c_reg), m_acc_reg, cycles);
        break;
    case INC_BC:
        inc_ss(m_b_reg, m_c_reg, cycles);
        break;
    case INC_B:
        inc_r(m_b_reg, m_flag_reg, cycles);
        break;
    case DEC_B:
        dec_r(m_b_reg, m_flag_reg, cycles);
        break;
    case LD_B_n:
        ld_r_n(m_b_reg, get_next_byte(), cycles);
        break;
    case RLCA:
        rlca(m_acc_reg, m_flag_reg, cycles);
        break;
    case LD_Mnn_SP:
        ld_Mnn_sp(m_sp, m_memory, get_next_word(), cycles);
        break;
    case ADD_HL_BC:
        add_HL_ss(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
        break;
    case LD_A_MBC:
        ld_A_Mss(m_acc_reg, m_memory.read(to_u16(m_b_reg, m_c_reg)), cycles);
        break;
    case DEC_BC:
        dec_ss(m_b_reg, m_c_reg, cycles);
        break;
    case INC_C:
        inc_r(m_c_reg, m_flag_reg, cycles);
        break;
    case DEC_C:
        dec_r(m_c_reg, m_flag_reg, cycles);
        break;
    case LD_C_n:
        ld_r_n(m_c_reg, get_next_byte(), cycles);
        break;
    case RRCA:
        rrca(m_acc_reg, m_flag_reg, cycles);
        break;
    case STOP_0:
        stop_0(cycles);
        break;
    case LD_DE_nn:
        ld_dd_nn(m_d_reg, m_e_reg, get_next_word(), cycles);
        break;
    case LD_MDE_A:
        ld_Mss_A(m_memory, to_u16(m_d_reg, m_e_reg), m_acc_reg, cycles);
        break;
    case INC_DE:
        inc_ss(m_d_reg, m_e_reg, cycles);
        break;
    case INC_D:
        inc_r(m_d_reg, m_flag_reg, cycles);
        break;
    case DEC_D:
        dec_r(m_d_reg, m_flag_reg, cycles);
        break;
    case LD_D_n:
        ld_r_n(m_d_reg, get_next_byte(), cycles);
        break;
    case RLA:
        rla(m_acc_reg, m_flag_reg, cycles);
        break;
    case JR_e:
        jr(m_pc, get_next_byte(), cycles);
        break;
    case ADD_HL_DE:
        add_HL_ss(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
        break;
    case LD_A_MDE:
        ld_A_Mss(m_acc_reg, m_memory.read(to_u16(m_d_reg, m_e_reg)), cycles);
        break;
    case DEC_DE:
        dec_ss(m_d_reg, m_e_reg, cycles);
        break;
    case INC_E:
        inc_r(m_e_reg, m_flag_reg, cycles);
        break;
    case DEC_E:
        dec_r(m_e_reg, m_flag_reg, cycles);
        break;
    case LD_E_n:
        ld_r_n(m_e_reg, get_next_byte(), cycles);
        break;
    case RRA:
        rra(m_acc_reg, m_flag_reg, cycles);
        break;
    case JR_NZ_e:
        jr_nz(m_pc, get_next_byte(), m_flag_reg, cycles);
        break;
    case LD_HL_nn:
        ld_dd_nn(m_h_reg, m_l_reg, get_next_word(), cycles);
        break;
    case LD_MHLp_A:
        ld_MHLp_A(m_memory, m_h_reg, m_l_reg, m_acc_reg, cycles);
        break;
    case INC_HL:
        inc_ss(m_h_reg, m_l_reg, cycles);
        break;
    case INC_H:
        inc_r(m_h_reg, m_flag_reg, cycles);
        break;
    case DEC_H:
        dec_r(m_h_reg, m_flag_reg, cycles);
        break;
    case LD_H_n:
        ld_r_n(m_h_reg, get_next_byte(), cycles);
        break;
    case DAA:
        daa(m_acc_reg, m_flag_reg, cycles);
        break;
    case JR_Z_e:
        jr_z(m_pc, get_next_byte(), m_flag_reg, cycles);
        break;
    case ADD_HL_HL:
        add_HL_ss(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
        break;
    case LD_A_MHLp:
        ld_A_MHLp(m_acc_reg, m_memory, m_h_reg, m_l_reg, cycles);
        break;
    case DEC_HL:
        dec_ss(m_h_reg, m_l_reg, cycles);
        break;
    case INC_L:
        inc_r(m_l_reg, m_flag_reg, cycles);
        break;
    case DEC_L:
        dec_r(m_l_reg, m_flag_reg, cycles);
        break;
    case LD_L_n:
        ld_r_n(m_l_reg, get_next_byte(), cycles);
        break;
    case CPL:
        cpl(m_acc_reg, m_flag_reg, cycles);
        break;
    case JR_NC_e:
        jr_nc(m_pc, get_next_byte(), m_flag_reg, cycles);
        break;
    case LD_SP_nn:
        ld_sp_nn(m_sp, get_next_word(), cycles);
        break;
    case LH_MHLm_A:
        ld_MHLm_A(m_memory, m_h_reg, m_l_reg, m_acc_reg, cycles);
        break;
    case INC_SP:
        inc_sp(m_sp, cycles);
        break;
    case INC_MHL:
        inc_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case DEC_MHL:
        dec_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case LD_MHL_n:
        ld_MHL_n(m_memory, address_in_HL(), get_next_byte(), cycles);
        break;
    case SCF:
        scf(m_flag_reg, cycles);
        break;
    case JR_C_e:
        jr_c(m_pc, get_next_byte(), m_flag_reg, cycles);
        break;
    case ADD_HL_SP:
        add_HL_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
        break;
    case LD_A_MHLm:
        ld_A_MHLm(m_acc_reg, m_memory, m_h_reg, m_l_reg, cycles);
        break;
    case DEC_SP:
        dec_sp(m_sp, cycles);
        break;
    case INC_A:
        inc_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case DEC_A:
        dec_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case LD_A_n:
        ld_r_n(m_acc_reg, get_next_byte(), cycles);
        break;
    case CCF:
        ccf(m_flag_reg, cycles);
        break;
    case LD_B_B:
        ld_r_r(m_b_reg, m_b_reg, cycles);
        break;
    case LD_B_C:
        ld_r_r(m_b_reg, m_c_reg, cycles);
        break;
    case LD_B_D:
        ld_r_r(m_b_reg, m_d_reg, cycles);
        break;
    case LD_B_E:
        ld_r_r(m_b_reg, m_e_reg, cycles);
        break;
    case LD_B_H:
        ld_r_r(m_b_reg, m_h_reg, cycles);
        break;
    case LD_B_L:
        ld_r_r(m_b_reg, m_l_reg, cycles);
        break;
    case LD_B_MHL:
        ld_r_MHL(m_b_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_B_A:
        ld_r_r(m_b_reg, m_acc_reg, cycles);
        break;
    case LD_C_B:
        ld_r_r(m_c_reg, m_b_reg, cycles);
        break;
    case LD_C_C:
        ld_r_r(m_c_reg, m_c_reg, cycles);
        break;
    case LD_C_D:
        ld_r_r(m_c_reg, m_d_reg, cycles);
        break;
    case LD_C_E:
        ld_r_r(m_c_reg, m_e_reg, cycles);
        break;
    case LD_C_H:
        ld_r_r(m_c_reg, m_h_reg, cycles);
        break;
    case LD_C_L:
        ld_r_r(m_c_reg, m_l_reg, cycles);
        break;
    case LD_C_MHL:
        ld_r_MHL(m_c_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_C_A:
        ld_r_r(m_c_reg, m_acc_reg, cycles);
        break;
    case LD_D_B:
        ld_r_r(m_d_reg, m_b_reg, cycles);
        break;
    case LD_D_C:
        ld_r_r(m_d_reg, m_c_reg, cycles);
        break;
    case LD_D_D:
        ld_r_r(m_d_reg, m_d_reg, cycles);
        break;
    case LD_D_E:
        ld_r_r(m_d_reg, m_e_reg, cycles);
        break;
    case LD_D_H:
        ld_r_r(m_d_reg, m_h_reg, cycles);
        break;
    case LD_D_L:
        ld_r_r(m_d_reg, m_l_reg, cycles);
        break;
    case LD_D_MHL:
        ld_r_MHL(m_d_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_D_A:
        ld_r_r(m_d_reg, m_acc_reg, cycles);
        break;
    case LD_E_B:
        ld_r_r(m_e_reg, m_b_reg, cycles);
        break;
    case LD_E_C:
        ld_r_r(m_e_reg, m_c_reg, cycles);
        break;
    case LD_E_D:
        ld_r_r(m_e_reg, m_d_reg, cycles);
        break;
    case LD_E_E:
        ld_r_r(m_e_reg, m_e_reg, cycles);
        break;
    case LD_E_H:
        ld_r_r(m_e_reg, m_h_reg, cycles);
        break;
    case LD_E_L:
        ld_r_r(m_e_reg, m_l_reg, cycles);
        break;
    case LD_E_MHL:
        ld_r_MHL(m_e_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_E_A:
        ld_r_r(m_e_reg, m_acc_reg, cycles);
        break;
    case LD_H_B:
        ld_r_r(m_h_reg, m_b_reg, cycles);
        break;
    case LD_H_C:
        ld_r_r(m_h_reg, m_c_reg, cycles);
        break;
    case LD_H_D:
        ld_r_r(m_h_reg, m_d_reg, cycles);
        break;
    case LD_H_E:
        ld_r_r(m_h_reg, m_e_reg, cycles);
        break;
    case LD_H_H:
        ld_r_r(m_h_reg, m_h_reg, cycles);
        break;
    case LD_H_L:
        ld_r_r(m_h_reg, m_l_reg, cycles);
        break;
    case LD_H_MHL:
        ld_r_MHL(m_h_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_H_A:
        ld_r_r(m_h_reg, m_acc_reg, cycles);
        break;
    case LD_L_B:
        ld_r_r(m_l_reg, m_b_reg, cycles);
        break;
    case LD_L_C:
        ld_r_r(m_l_reg, m_c_reg, cycles);
        break;
    case LD_L_D:
        ld_r_r(m_l_reg, m_d_reg, cycles);
        break;
    case LD_L_E:
        ld_r_r(m_l_reg, m_e_reg, cycles);
        break;
    case LD_L_H:
        ld_r_r(m_l_reg, m_h_reg, cycles);
        break;
    case LD_L_L:
        ld_r_r(m_l_reg, m_l_reg, cycles);
        break;
    case LD_L_MHL:
        ld_r_MHL(m_l_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_L_A:
        ld_r_r(m_l_reg, m_acc_reg, cycles);
        break;
    case LD_MHL_B:
        ld_MHL_r(m_memory, address_in_HL(), m_b_reg, cycles);
        break;
    case LD_MHL_C:
        ld_MHL_r(m_memory, address_in_HL(), m_c_reg, cycles);
        break;
    case LD_MHL_D:
        ld_MHL_r(m_memory, address_in_HL(), m_d_reg, cycles);
        break;
    case LD_MHL_E:
        ld_MHL_r(m_memory, address_in_HL(), m_e_reg, cycles);
        break;
    case LD_MHL_H:
        ld_MHL_r(m_memory, address_in_HL(), m_h_reg, cycles);
        break;
    case LD_MHL_L:
        ld_MHL_r(m_memory, address_in_HL(), m_l_reg, cycles);
        break;
    case HALT:
        halt(m_is_halted, cycles);
        break;
    case LD_MHL_A:
        ld_MHL_r(m_memory, address_in_HL(), m_acc_reg, cycles);
        break;
    case LD_A_B:
        ld_r_r(m_acc_reg, m_b_reg, cycles);
        break;
    case LD_A_C:
        ld_r_r(m_acc_reg, m_c_reg, cycles);
        break;
    case LD_A_D:
        ld_r_r(m_acc_reg, m_d_reg, cycles);
        break;
    case LD_A_E:
        ld_r_r(m_acc_reg, m_e_reg, cycles);
        break;
    case LD_A_H:
        ld_r_r(m_acc_reg, m_h_reg, cycles);
        break;
    case LD_A_L:
        ld_r_r(m_acc_reg, m_l_reg, cycles);
        break;
    case LD_A_MHL:
        ld_r_MHL(m_acc_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case LD_A_A:
        ld_r_r(m_acc_reg, m_acc_reg, cycles);
        break;
    case ADD_A_B:
        add_A_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADD_A_C:
        add_A_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADD_A_D:
        add_A_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADD_A_E:
        add_A_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADD_A_H:
        add_A_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ADD_A_L:
        add_A_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ADD_A_MHL:
        add_A_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ADD_A_A:
        add_A_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case ADC_A_B:
        adc_A_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADC_A_C:
        adc_A_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADC_A_D:
        adc_A_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADC_A_E:
        adc_A_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADC_A_H:
        adc_A_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ADC_A_L:
        adc_A_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ADC_A_MHL:
        adc_A_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ADC_A_A:
        adc_A_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case SUB_B:
        sub_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case SUB_C:
        sub_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case SUB_D:
        sub_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case SUB_E:
        sub_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case SUB_H:
        sub_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case SUB_L:
        sub_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case SUB_MHL:
        sub_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case SUB_A:
        sub_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case SBC_A_B:
        sbc_A_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case SBC_A_C:
        sbc_A_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case SBC_A_D:
        sbc_A_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case SBC_A_E:
        sbc_A_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case SBC_A_H:
        sbc_A_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case SBC_A_L:
        sbc_A_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case SBC_A_MHL:
        sbc_A_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case SBC_A_A:
        sbc_A_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case AND_B:
        and_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case AND_C:
        and_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case AND_D:
        and_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case AND_E:
        and_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case AND_H:
        and_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case AND_L:
        and_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case AND_MHL:
        and_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case AND_A:
        and_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case XOR_B:
        xor_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case XOR_C:
        xor_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case XOR_D:
        xor_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case XOR_E:
        xor_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case XOR_H:
        xor_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case XOR_L:
        xor_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case XOR_MHL:
        xor_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case XOR_A:
        xor_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case OR_B:
        or_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case OR_C:
        or_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case OR_D:
        or_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case OR_E:
        or_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case OR_H:
        or_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case OR_L:
        or_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case OR_MHL:
        or_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case OR_A:
        or_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case CP_B:
        cp_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case CP_C:
        cp_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case CP_D:
        cp_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case CP_E:
        cp_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case CP_H:
        cp_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case CP_L:
        cp_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case CP_MHL:
        cp_MHL(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case CP_A:
        cp_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case RET_NZ:
        ret_nz(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_BC:
        pop(m_b_reg, m_c_reg, m_sp, m_memory, cycles);
        break;
    case JP_NZ:
        jp_nz(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case JP:
        jp(m_pc, get_next_word(), cycles);
        break;
    case CALL_NZ:
        call_nz(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_BC:
        push_qq(m_b_reg, m_c_reg, m_sp, m_memory, cycles);
        break;
    case ADD_A_n:
        add_A_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_0:
        rst_0(m_pc, m_sp, m_memory, cycles);
        break;
    case RET_Z:
        ret_z(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case RET:
        ret(m_pc, m_sp, m_memory, cycles);
        break;
    case JP_Z:
        jp_z(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case BITS:
        next_bits_instruction(get_next_byte().farg, cycles);
        break;
    case CALL_Z:
        call_z(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case CALL:
        call(m_pc, m_sp, m_memory, get_next_word(), cycles);
        break;
    case ADC_A_n:
        adc_A_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_1:
        rst_1(m_pc, m_sp, m_memory, cycles);
        break;
    case RET_NC:
        ret_nc(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_DE:
        pop(m_d_reg, m_e_reg, m_sp, m_memory, cycles);
        break;
    case JP_NC:
        jp_nc(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case CALL_NC:
        call_nc(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_DE:
        push_qq(m_d_reg, m_e_reg, m_sp, m_memory, cycles);
        break;
    case SUB_n:
        sub_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_2:
        rst_2(m_pc, m_sp, m_memory, cycles);
        break;
    case RET_C:
        ret_c(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case RETI:
        reti(m_pc, m_sp, m_memory, cycles);
        break;
    case JP_C:
        jp_c(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case CALL_C:
        call_c(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case SBC_A_n:
        sbc_A_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_3:
        rst_3(m_pc, m_sp, m_memory, cycles);
        break;
    case LDH_Mn_A:
        ldh_Mn_A(m_memory, get_next_byte(), m_acc_reg, cycles);
        break;
    case POP_HL:
        pop(m_h_reg, m_l_reg, m_sp, m_memory, cycles);
        break;
    case LD_MC_A:
        ld_MC_A(m_c_reg, m_acc_reg, m_memory, cycles);
        break;
    case PUSH_HL:
        push_qq(m_h_reg, m_l_reg, m_sp, m_memory, cycles);
        break;
    case AND_n:
        and_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_4:
        rst_4(m_pc, m_sp, m_memory, cycles);
        break;
    case ADD_SP_n:
        add_SP_n(m_sp, get_next_byte(), m_flag_reg, cycles);
        break;
    case JP_MHL:
        jp_hl(m_pc, address_in_HL(), cycles);
        break;
    case LD_Mnn_A:
        ld_Mnn_A(m_acc_reg, m_memory, get_next_word(), cycles);
        break;
    case XOR_n:
        xor_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_5:
        rst_5(m_pc, m_sp, m_memory, cycles);
        break;
    case LDH_A_Mn:
        ldh_A_Mn(m_acc_reg, m_memory, get_next_byte(), cycles);
        break;
    case POP_AF:
        pop_af(m_flag_reg, m_acc_reg, m_sp, m_memory, cycles);
        break;
    case LD_A_MC:
        ld_A_MC(m_acc_reg, m_c_reg, m_memory, cycles);
        break;
    case DI:
        di(m_ime, cycles);
        break;
    case PUSH_AF:
        push_af(m_flag_reg, m_acc_reg, m_sp, m_memory, cycles);
        break;
    case OR_n:
        or_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_6:
        rst_6(m_pc, m_sp, m_memory, cycles);
        break;
    case LD_HL_SPpn:
        ld_HL_SP_p_n(m_h_reg, m_l_reg, m_sp, get_next_byte(), m_flag_reg, cycles);
        break;
    case LD_SP_HL:
        ld_sp_hl(m_sp, address_in_HL(), cycles);
        break;
    case LD_A_Mnn:
        ld_A_Mnn(m_acc_reg, m_memory, get_next_word(), cycles);
        break;
    case EI:
        ei(m_ime, cycles);
        break;
    case CP_n:
        cp_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_7:
        rst_7(m_pc, m_sp, m_memory, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(m_opcode);
    }

    return cycles;
}

void Cpu::next_bits_instruction(u8 bits_opcode, cyc& cycles)
{
    print_debug(bits_opcode);

    switch (bits_opcode) {
    case RLC_B:
        rlc_r(m_b_reg, m_flag_reg, cycles);
        break;
    case RLC_C:
        rlc_r(m_c_reg, m_flag_reg, cycles);
        break;
    case RLC_D:
        rlc_r(m_d_reg, m_flag_reg, cycles);
        break;
    case RLC_E:
        rlc_r(m_e_reg, m_flag_reg, cycles);
        break;
    case RLC_H:
        rlc_r(m_h_reg, m_flag_reg, cycles);
        break;
    case RLC_L:
        rlc_r(m_l_reg, m_flag_reg, cycles);
        break;
    case RLC_MHL:
        rlc_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case RLC_A:
        rlc_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case RRC_B:
        rrc_r(m_b_reg, m_flag_reg, cycles);
        break;
    case RRC_C:
        rrc_r(m_c_reg, m_flag_reg, cycles);
        break;
    case RRC_D:
        rrc_r(m_d_reg, m_flag_reg, cycles);
        break;
    case RRC_E:
        rrc_r(m_e_reg, m_flag_reg, cycles);
        break;
    case RRC_H:
        rrc_r(m_h_reg, m_flag_reg, cycles);
        break;
    case RRC_L:
        rrc_r(m_l_reg, m_flag_reg, cycles);
        break;
    case RRC_MHL:
        rrc_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case RRC_A:
        rrc_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case RL_B:
        rl_r(m_b_reg, m_flag_reg, cycles);
        break;
    case RL_C:
        rl_r(m_c_reg, m_flag_reg, cycles);
        break;
    case RL_D:
        rl_r(m_d_reg, m_flag_reg, cycles);
        break;
    case RL_E:
        rl_r(m_e_reg, m_flag_reg, cycles);
        break;
    case RL_H:
        rl_r(m_h_reg, m_flag_reg, cycles);
        break;
    case RL_L:
        rl_r(m_l_reg, m_flag_reg, cycles);
        break;
    case RL_MHL:
        rl_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case RL_A:
        rl_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case RR_B:
        rr_r(m_b_reg, m_flag_reg, cycles);
        break;
    case RR_C:
        rr_r(m_c_reg, m_flag_reg, cycles);
        break;
    case RR_D:
        rr_r(m_d_reg, m_flag_reg, cycles);
        break;
    case RR_E:
        rr_r(m_e_reg, m_flag_reg, cycles);
        break;
    case RR_H:
        rr_r(m_h_reg, m_flag_reg, cycles);
        break;
    case RR_L:
        rr_r(m_l_reg, m_flag_reg, cycles);
        break;
    case RR_MHL:
        rr_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case RR_A:
        rr_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case SLA_B:
        sla_r(m_b_reg, m_flag_reg, cycles);
        break;
    case SLA_C:
        sla_r(m_c_reg, m_flag_reg, cycles);
        break;
    case SLA_D:
        sla_r(m_d_reg, m_flag_reg, cycles);
        break;
    case SLA_E:
        sla_r(m_e_reg, m_flag_reg, cycles);
        break;
    case SLA_H:
        sla_r(m_h_reg, m_flag_reg, cycles);
        break;
    case SLA_L:
        sla_r(m_l_reg, m_flag_reg, cycles);
        break;
    case SLA_MHL:
        sla_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case SLA_A:
        sla_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case SRA_B:
        sra_r(m_b_reg, m_flag_reg, cycles);
        break;
    case SRA_C:
        sra_r(m_c_reg, m_flag_reg, cycles);
        break;
    case SRA_D:
        sra_r(m_d_reg, m_flag_reg, cycles);
        break;
    case SRA_E:
        sra_r(m_e_reg, m_flag_reg, cycles);
        break;
    case SRA_H:
        sra_r(m_h_reg, m_flag_reg, cycles);
        break;
    case SRA_L:
        sra_r(m_l_reg, m_flag_reg, cycles);
        break;
    case SRA_MHL:
        sra_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case SRA_A:
        sra_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case SWAP_B:
        swap(m_b_reg, m_flag_reg, cycles);
        break;
    case SWAP_C:
        swap(m_c_reg, m_flag_reg, cycles);
        break;
    case SWAP_D:
        swap(m_d_reg, m_flag_reg, cycles);
        break;
    case SWAP_E:
        swap(m_e_reg, m_flag_reg, cycles);
        break;
    case SWAP_H:
        swap(m_h_reg, m_flag_reg, cycles);
        break;
    case SWAP_L:
        swap(m_l_reg, m_flag_reg, cycles);
        break;
    case SWAP_MHL:
        swap_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case SWAP_A:
        swap(m_acc_reg, m_flag_reg, cycles);
        break;
    case SRL_B:
        srl_r(m_b_reg, m_flag_reg, cycles);
        break;
    case SRL_C:
        srl_r(m_c_reg, m_flag_reg, cycles);
        break;
    case SRL_D:
        srl_r(m_d_reg, m_flag_reg, cycles);
        break;
    case SRL_E:
        srl_r(m_e_reg, m_flag_reg, cycles);
        break;
    case SRL_H:
        srl_r(m_h_reg, m_flag_reg, cycles);
        break;
    case SRL_L:
        srl_r(m_l_reg, m_flag_reg, cycles);
        break;
    case SRL_MHL:
        srl_MHL(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case SRL_A:
        srl_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_0_B:
        bit_r(0, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_0_C:
        bit_r(0, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_0_D:
        bit_r(0, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_0_E:
        bit_r(0, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_0_H:
        bit_r(0, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_0_L:
        bit_r(0, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_0_MHL:
        bit_MHL(0, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_0_A:
        bit_r(0, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_1_B:
        bit_r(1, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_1_C:
        bit_r(1, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_1_D:
        bit_r(1, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_1_E:
        bit_r(1, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_1_H:
        bit_r(1, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_1_L:
        bit_r(1, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_1_MHL:
        bit_MHL(1, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_1_A:
        bit_r(1, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_2_B:
        bit_r(2, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_2_C:
        bit_r(2, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_2_D:
        bit_r(2, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_2_E:
        bit_r(2, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_2_H:
        bit_r(2, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_2_L:
        bit_r(2, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_2_MHL:
        bit_MHL(2, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_2_A:
        bit_r(2, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_3_B:
        bit_r(3, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_3_C:
        bit_r(3, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_3_D:
        bit_r(3, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_3_E:
        bit_r(3, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_3_H:
        bit_r(3, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_3_L:
        bit_r(3, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_3_MHL:
        bit_MHL(3, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_3_A:
        bit_r(3, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_4_B:
        bit_r(4, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_4_C:
        bit_r(4, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_4_D:
        bit_r(4, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_4_E:
        bit_r(4, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_4_H:
        bit_r(4, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_4_L:
        bit_r(4, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_4_MHL:
        bit_MHL(4, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_4_A:
        bit_r(4, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_5_B:
        bit_r(5, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_5_C:
        bit_r(5, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_5_D:
        bit_r(5, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_5_E:
        bit_r(5, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_5_H:
        bit_r(5, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_5_L:
        bit_r(5, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_5_MHL:
        bit_MHL(5, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_5_A:
        bit_r(5, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_6_B:
        bit_r(6, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_6_C:
        bit_r(6, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_6_D:
        bit_r(6, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_6_E:
        bit_r(6, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_6_H:
        bit_r(6, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_6_L:
        bit_r(6, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_6_MHL:
        bit_MHL(6, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_6_A:
        bit_r(6, m_acc_reg, m_flag_reg, cycles);
        break;
    case BIT_7_B:
        bit_r(7, m_b_reg, m_flag_reg, cycles);
        break;
    case BIT_7_C:
        bit_r(7, m_c_reg, m_flag_reg, cycles);
        break;
    case BIT_7_D:
        bit_r(7, m_d_reg, m_flag_reg, cycles);
        break;
    case BIT_7_E:
        bit_r(7, m_e_reg, m_flag_reg, cycles);
        break;
    case BIT_7_H:
        bit_r(7, m_h_reg, m_flag_reg, cycles);
        break;
    case BIT_7_L:
        bit_r(7, m_l_reg, m_flag_reg, cycles);
        break;
    case BIT_7_MHL:
        bit_MHL(7, address_in_HL(), m_memory, m_flag_reg, cycles);
        break;
    case BIT_7_A:
        bit_r(7, m_acc_reg, m_flag_reg, cycles);
        break;
    case RES_0_B:
        res_r(0, m_b_reg, cycles);
        break;
    case RES_0_C:
        res_r(0, m_c_reg, cycles);
        break;
    case RES_0_D:
        res_r(0, m_d_reg, cycles);
        break;
    case RES_0_E:
        res_r(0, m_e_reg, cycles);
        break;
    case RES_0_H:
        res_r(0, m_h_reg, cycles);
        break;
    case RES_0_L:
        res_r(0, m_l_reg, cycles);
        break;
    case RES_0_MHL:
        res_MHL(0, address_in_HL(), m_memory, cycles);
        break;
    case RES_0_A:
        res_r(0, m_acc_reg, cycles);
        break;
    case RES_1_B:
        res_r(1, m_b_reg, cycles);
        break;
    case RES_1_C:
        res_r(1, m_c_reg, cycles);
        break;
    case RES_1_D:
        res_r(1, m_d_reg, cycles);
        break;
    case RES_1_E:
        res_r(1, m_e_reg, cycles);
        break;
    case RES_1_H:
        res_r(1, m_h_reg, cycles);
        break;
    case RES_1_L:
        res_r(1, m_l_reg, cycles);
        break;
    case RES_1_MHL:
        res_MHL(1, address_in_HL(), m_memory, cycles);
        break;
    case RES_1_A:
        res_r(1, m_acc_reg, cycles);
        break;
    case RES_2_B:
        res_r(2, m_b_reg, cycles);
        break;
    case RES_2_C:
        res_r(2, m_c_reg, cycles);
        break;
    case RES_2_D:
        res_r(2, m_d_reg, cycles);
        break;
    case RES_2_E:
        res_r(2, m_e_reg, cycles);
        break;
    case RES_2_H:
        res_r(2, m_h_reg, cycles);
        break;
    case RES_2_L:
        res_r(2, m_l_reg, cycles);
        break;
    case RES_2_MHL:
        res_MHL(2, address_in_HL(), m_memory, cycles);
        break;
    case RES_2_A:
        res_r(2, m_acc_reg, cycles);
        break;
    case RES_3_B:
        res_r(3, m_b_reg, cycles);
        break;
    case RES_3_C:
        res_r(3, m_c_reg, cycles);
        break;
    case RES_3_D:
        res_r(3, m_d_reg, cycles);
        break;
    case RES_3_E:
        res_r(3, m_e_reg, cycles);
        break;
    case RES_3_H:
        res_r(3, m_h_reg, cycles);
        break;
    case RES_3_L:
        res_r(3, m_l_reg, cycles);
        break;
    case RES_3_MHL:
        res_MHL(3, address_in_HL(), m_memory, cycles);
        break;
    case RES_3_A:
        res_r(3, m_acc_reg, cycles);
        break;
    case RES_4_B:
        res_r(4, m_b_reg, cycles);
        break;
    case RES_4_C:
        res_r(4, m_c_reg, cycles);
        break;
    case RES_4_D:
        res_r(4, m_d_reg, cycles);
        break;
    case RES_4_E:
        res_r(4, m_e_reg, cycles);
        break;
    case RES_4_H:
        res_r(4, m_h_reg, cycles);
        break;
    case RES_4_L:
        res_r(4, m_l_reg, cycles);
        break;
    case RES_4_MHL:
        res_MHL(4, address_in_HL(), m_memory, cycles);
        break;
    case RES_4_A:
        res_r(4, m_acc_reg, cycles);
        break;
    case RES_5_B:
        res_r(5, m_b_reg, cycles);
        break;
    case RES_5_C:
        res_r(5, m_c_reg, cycles);
        break;
    case RES_5_D:
        res_r(5, m_d_reg, cycles);
        break;
    case RES_5_E:
        res_r(5, m_e_reg, cycles);
        break;
    case RES_5_H:
        res_r(5, m_h_reg, cycles);
        break;
    case RES_5_L:
        res_r(5, m_l_reg, cycles);
        break;
    case RES_5_MHL:
        res_MHL(5, address_in_HL(), m_memory, cycles);
        break;
    case RES_5_A:
        res_r(5, m_acc_reg, cycles);
        break;
    case RES_6_B:
        res_r(6, m_b_reg, cycles);
        break;
    case RES_6_C:
        res_r(6, m_c_reg, cycles);
        break;
    case RES_6_D:
        res_r(6, m_d_reg, cycles);
        break;
    case RES_6_E:
        res_r(6, m_e_reg, cycles);
        break;
    case RES_6_H:
        res_r(6, m_h_reg, cycles);
        break;
    case RES_6_L:
        res_r(6, m_l_reg, cycles);
        break;
    case RES_6_MHL:
        res_MHL(6, address_in_HL(), m_memory, cycles);
        break;
    case RES_6_A:
        res_r(6, m_acc_reg, cycles);
        break;
    case RES_7_B:
        res_r(7, m_b_reg, cycles);
        break;
    case RES_7_C:
        res_r(7, m_c_reg, cycles);
        break;
    case RES_7_D:
        res_r(7, m_d_reg, cycles);
        break;
    case RES_7_E:
        res_r(7, m_e_reg, cycles);
        break;
    case RES_7_H:
        res_r(7, m_h_reg, cycles);
        break;
    case RES_7_L:
        res_r(7, m_l_reg, cycles);
        break;
    case RES_7_MHL:
        res_MHL(7, address_in_HL(), m_memory, cycles);
        break;
    case RES_7_A:
        res_r(7, m_acc_reg, cycles);
        break;
    case SET_0_B:
        set_r(0, m_b_reg, cycles);
        break;
    case SET_0_C:
        set_r(0, m_c_reg, cycles);
        break;
    case SET_0_D:
        set_r(0, m_d_reg, cycles);
        break;
    case SET_0_E:
        set_r(0, m_e_reg, cycles);
        break;
    case SET_0_H:
        set_r(0, m_h_reg, cycles);
        break;
    case SET_0_L:
        set_r(0, m_l_reg, cycles);
        break;
    case SET_0_MHL:
        set_MHL(0, address_in_HL(), m_memory, cycles);
        break;
    case SET_0_A:
        set_r(0, m_acc_reg, cycles);
        break;
    case SET_1_B:
        set_r(1, m_b_reg, cycles);
        break;
    case SET_1_C:
        set_r(1, m_c_reg, cycles);
        break;
    case SET_1_D:
        set_r(1, m_d_reg, cycles);
        break;
    case SET_1_E:
        set_r(1, m_e_reg, cycles);
        break;
    case SET_1_H:
        set_r(1, m_h_reg, cycles);
        break;
    case SET_1_L:
        set_r(1, m_l_reg, cycles);
        break;
    case SET_1_MHL:
        set_MHL(1, address_in_HL(), m_memory, cycles);
        break;
    case SET_1_A:
        set_r(1, m_acc_reg, cycles);
        break;
    case SET_2_B:
        set_r(2, m_b_reg, cycles);
        break;
    case SET_2_C:
        set_r(2, m_c_reg, cycles);
        break;
    case SET_2_D:
        set_r(2, m_d_reg, cycles);
        break;
    case SET_2_E:
        set_r(2, m_e_reg, cycles);
        break;
    case SET_2_H:
        set_r(2, m_h_reg, cycles);
        break;
    case SET_2_L:
        set_r(2, m_l_reg, cycles);
        break;
    case SET_2_MHL:
        set_MHL(2, address_in_HL(), m_memory, cycles);
        break;
    case SET_2_A:
        set_r(2, m_acc_reg, cycles);
        break;
    case SET_3_B:
        set_r(3, m_b_reg, cycles);
        break;
    case SET_3_C:
        set_r(3, m_c_reg, cycles);
        break;
    case SET_3_D:
        set_r(3, m_d_reg, cycles);
        break;
    case SET_3_E:
        set_r(3, m_e_reg, cycles);
        break;
    case SET_3_H:
        set_r(3, m_h_reg, cycles);
        break;
    case SET_3_L:
        set_r(3, m_l_reg, cycles);
        break;
    case SET_3_MHL:
        set_MHL(3, address_in_HL(), m_memory, cycles);
        break;
    case SET_3_A:
        set_r(3, m_acc_reg, cycles);
        break;
    case SET_4_B:
        set_r(4, m_b_reg, cycles);
        break;
    case SET_4_C:
        set_r(4, m_c_reg, cycles);
        break;
    case SET_4_D:
        set_r(4, m_d_reg, cycles);
        break;
    case SET_4_E:
        set_r(4, m_e_reg, cycles);
        break;
    case SET_4_H:
        set_r(4, m_h_reg, cycles);
        break;
    case SET_4_L:
        set_r(4, m_l_reg, cycles);
        break;
    case SET_4_MHL:
        set_MHL(4, address_in_HL(), m_memory, cycles);
        break;
    case SET_4_A:
        set_r(4, m_acc_reg, cycles);
        break;
    case SET_5_B:
        set_r(5, m_b_reg, cycles);
        break;
    case SET_5_C:
        set_r(5, m_c_reg, cycles);
        break;
    case SET_5_D:
        set_r(5, m_d_reg, cycles);
        break;
    case SET_5_E:
        set_r(5, m_e_reg, cycles);
        break;
    case SET_5_H:
        set_r(5, m_h_reg, cycles);
        break;
    case SET_5_L:
        set_r(5, m_l_reg, cycles);
        break;
    case SET_5_MHL:
        set_MHL(5, address_in_HL(), m_memory, cycles);
        break;
    case SET_5_A:
        set_r(5, m_acc_reg, cycles);
        break;
    case SET_6_B:
        set_r(6, m_b_reg, cycles);
        break;
    case SET_6_C:
        set_r(6, m_c_reg, cycles);
        break;
    case SET_6_D:
        set_r(6, m_d_reg, cycles);
        break;
    case SET_6_E:
        set_r(6, m_e_reg, cycles);
        break;
    case SET_6_H:
        set_r(6, m_h_reg, cycles);
        break;
    case SET_6_L:
        set_r(6, m_l_reg, cycles);
        break;
    case SET_6_MHL:
        set_MHL(6, address_in_HL(), m_memory, cycles);
        break;
    case SET_6_A:
        set_r(6, m_acc_reg, cycles);
        break;
    case SET_7_B:
        set_r(7, m_b_reg, cycles);
        break;
    case SET_7_C:
        set_r(7, m_c_reg, cycles);
        break;
    case SET_7_D:
        set_r(7, m_d_reg, cycles);
        break;
    case SET_7_E:
        set_r(7, m_e_reg, cycles);
        break;
    case SET_7_H:
        set_r(7, m_h_reg, cycles);
        break;
    case SET_7_L:
        set_r(7, m_l_reg, cycles);
        break;
    case SET_7_MHL:
        set_MHL(7, address_in_HL(), m_memory, cycles);
        break;
    case SET_7_A:
        set_r(7, m_acc_reg, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(bits_opcode, "Bits instructions");
    }
}

cyc Cpu::handle_interrupt(cyc cycles)
{
    m_ime = m_ie = false;
    m_is_halted = false;

    m_pc = m_pc_from_interruptor;

    cycles = 13;

    return cycles;
}

NextByte Cpu::get_next_byte()
{
    return {
        .farg = m_memory.read(m_pc++)
    };
}

NextWord Cpu::get_next_word()
{
    return {
        .farg = m_memory.read(m_pc++),
        .sarg = m_memory.read(m_pc++)
    };
}

u16 Cpu::address_in_HL() const
{
    return to_u16(m_h_reg, m_l_reg);
}

EmulatorMemory<u16, u8>& Cpu::memory()
{
    return m_memory;
}

u16 Cpu::pc() const
{
    return m_pc;
}

u16 Cpu::sp() const
{
    return m_sp;
}

u8 Cpu::a() const
{
    return m_acc_reg;
}

u8 Cpu::b() const
{
    return m_b_reg;
}

u8 Cpu::c() const
{
    return m_c_reg;
}

u8 Cpu::d() const
{
    return m_d_reg;
}

u8 Cpu::e() const
{
    return m_e_reg;
}

u8 Cpu::h() const
{
    return m_h_reg;
}

u8 Cpu::l() const
{
    return m_l_reg;
}

u8 Cpu::f() const
{
    return m_flag_reg.to_u8();
}

bool Cpu::ime() const
{
    return m_ime;
}

bool Cpu::ie() const
{
    return m_ie;
}

void Cpu::print_debug(u8 opcode)
{
    if (false) {
        std::cout << "pc=" << hexify(static_cast<u16>(m_pc - 1)) // -1 because fetching opcode increments by one
                  << ",sp=" << hexify(m_sp)
                  << ",op=" << hexify(opcode)
                  << ",a=" << hexify(m_acc_reg)
                  << ",b=" << hexify(m_b_reg)
                  << ",c=" << hexify(m_c_reg)
                  << ",d=" << hexify(m_d_reg)
                  << ",e=" << hexify(m_e_reg)
                  << ",h=" << hexify(m_h_reg)
                  << ",l=" << hexify(m_l_reg)
                  << ",c=" << m_flag_reg.is_carry_flag_set()
                  << ",hc=" << m_flag_reg.is_half_carry_flag_set()
                  << ",n=" << m_flag_reg.is_add_subtract_flag_set()
                  << ",z=" << m_flag_reg.is_zero_flag_set()
                  << "\n"
                  << std::flush;
    }
}
}
