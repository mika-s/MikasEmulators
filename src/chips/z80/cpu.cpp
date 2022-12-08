#include "cpu.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

namespace emu::z80 {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::string::hexify;

Cpu::Cpu(
    EmulatorMemory<u16, u8>& memory,
    const u16 initial_pc)
    : m_is_halted(false)
    , m_iff1(false)
    , m_iff2(false)
    , m_is_interrupted(false)
    , m_is_nmi_interrupted(false)
    , m_instruction_from_interruptor(0)
    , m_memory(memory)
    , m_memory_size(memory.size())
    , m_io_in(number_of_io_ports)
    , m_io_out(number_of_io_ports)
    , m_opcode(0)
    , m_sp(0xffff)
    , m_pc(initial_pc)
    , m_acc_reg(0xff)
    , m_acc_p_reg(0)
    , m_b_reg(0)
    , m_b_p_reg(0)
    , m_c_reg(0)
    , m_c_p_reg(0)
    , m_d_reg(0)
    , m_d_p_reg(0)
    , m_e_reg(0)
    , m_e_p_reg(0)
    , m_h_reg(0)
    , m_h_p_reg(0)
    , m_l_reg(0)
    , m_l_p_reg(0)
    , m_ix_reg(0)
    , m_iy_reg(0)
    , m_i_reg(0)
    , m_r_reg(0)
    , m_interrupt_mode(InterruptMode::ZERO)
{
    m_flag_reg.from_u8(0xff);
    m_flag_p_reg.from_u8(0x00);
}

Cpu::~Cpu()
{
    m_out_observers.clear();
    m_in_observers.clear();
}

void Cpu::add_out_observer(OutObserver& observer)
{
    m_out_observers.push_back(&observer);
}

void Cpu::remove_out_observer(OutObserver* observer)
{
    m_out_observers.erase(
        std::remove(m_out_observers.begin(), m_out_observers.end(), observer),
        m_out_observers.end());
}

void Cpu::add_in_observer(InObserver& observer)
{
    m_in_observers.push_back(&observer);
}

void Cpu::remove_in_observer(InObserver* observer)
{
    m_in_observers.erase(
        std::remove(m_in_observers.begin(), m_in_observers.end(), observer),
        m_in_observers.end());
}

bool Cpu::can_run_next_instruction() const
{
    return m_pc < m_memory_size;
}

void Cpu::reset_state()
{
    m_acc_reg = 0xff;
    m_acc_p_reg = 0;
    m_b_reg = m_b_p_reg = 0;
    m_c_reg = m_c_p_reg = 0;
    m_d_reg = m_d_p_reg = 0;
    m_e_reg = m_e_p_reg = 0;
    m_h_reg = m_h_p_reg = 0;
    m_l_reg = m_l_p_reg = 0;
    m_ix_reg = m_iy_reg = 0;
    m_i_reg = m_r_reg = 0;
    m_flag_reg.from_u8(0xff);
    m_flag_p_reg.from_u8(0x00);
    m_pc = 0;
    m_sp = 0xffff;
    m_is_halted = false;
    m_iff1 = m_iff2 = false;
    m_is_interrupted = false;
    m_is_nmi_interrupted = false;
    m_instruction_from_interruptor = 0;
    m_interrupt_mode = InterruptMode::ZERO;
    std::fill(m_io_in.begin(), m_io_in.end(), 0);
    std::fill(m_io_out.begin(), m_io_out.end(), 0);
}

void Cpu::start()
{
}

void Cpu::stop()
{
    reset_state();
}

void Cpu::interrupt(u8 instruction_to_perform)
{
    m_is_interrupted = true;
    m_instruction_from_interruptor = instruction_to_perform;
}

void Cpu::nmi_interrupt()
{
    m_is_nmi_interrupted = true;
}

bool Cpu::is_inta() const
{
    return m_iff1;
}

void Cpu::input(u8 port, u8 value)
{
    m_io_in[port] = value;
}

cyc Cpu::next_instruction()
{
    cyc cycles = 0;

    if (m_is_nmi_interrupted) {
        return handle_nonmaskable_interrupt(cycles);
    } else if (m_iff1 && m_is_interrupted && m_interrupt_mode == InterruptMode::ZERO) {
        cycles += handle_maskable_interrupt_0(cycles);
    } else if (m_iff1 && m_is_interrupted) {
        return handle_maskable_interrupt_1_2(cycles);
    } else if (m_is_halted) {
        return 4; // TODO: What is the proper value while NOPing during halt?
    }

    m_opcode = get_next_byte().farg;

    print_debug(m_opcode);

    r_tick();

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
    case EX_AF_AFP:
        ex(m_acc_reg, m_flag_reg, m_acc_p_reg, m_flag_p_reg, cycles);
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
    case DJNZ:
        djnz(m_b_reg, m_pc, get_next_byte(), cycles);
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
    case LD_Mnn_HL:
        ld_Mnn_HL(m_h_reg, m_l_reg, m_memory, get_next_word(), cycles);
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
    case LD_HL_Mnn:
        ld_HL_Mnn(m_h_reg, m_l_reg, m_memory, get_next_word(), cycles);
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
    case LD_Mnn_A:
        ld_Mnn_A(m_acc_reg, m_memory, get_next_word(), cycles);
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
        scf(m_flag_reg, m_acc_reg, cycles);
        break;
    case JR_C_e:
        jr_c(m_pc, get_next_byte(), m_flag_reg, cycles);
        break;
    case ADD_HL_SP:
        add_HL_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
        break;
    case LD_A_Mnn:
        ld_A_Mnn(m_acc_reg, m_memory, get_next_word(), cycles);
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
        ccf(m_flag_reg, m_acc_reg, cycles);
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
    case OUT: {
        NextByte args = get_next_byte();
        out_n_A(m_acc_reg, args, m_io_out, cycles);
        notify_out_observers(args.farg);
        break;
    }
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
    case EXX:
        exx(m_b_reg, m_c_reg, m_b_p_reg, m_c_p_reg,
            m_d_reg, m_e_reg, m_d_p_reg, m_e_p_reg,
            m_h_reg, m_l_reg, m_h_p_reg, m_l_p_reg,
            cycles);
        break;
    case JP_C:
        jp_c(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case IN: {
        NextByte args = get_next_byte();
        notify_in_observers(args.farg);
        in_A_n(m_acc_reg, args, m_io_in, cycles);
        break;
    }
    case CALL_C:
        call_c(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case IX:
        next_ixy_instruction(get_next_byte().farg, m_ix_reg, cycles);
        break;
    case SBC_A_n:
        sbc_A_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_3:
        rst_3(m_pc, m_sp, m_memory, cycles);
        break;
    case RET_PO:
        ret_po(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_HL:
        pop(m_h_reg, m_l_reg, m_sp, m_memory, cycles);
        break;
    case JP_PO:
        jp_po(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case EX_MSP_HL:
        ex_msp_hl(m_sp, memory(), m_h_reg, m_l_reg, cycles);
        break;
    case CALL_PO:
        call_po(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
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
    case RET_PE:
        ret_pe(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case JP_MHL:
        jp_hl(m_pc, address_in_HL(), cycles);
        break;
    case JP_PE:
        jp_pe(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case EX_DE_HL:
        ex_de_hl(m_h_reg, m_l_reg, m_d_reg, m_e_reg, cycles);
        break;
    case CALL_PE:
        call_pe(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case EXTD:
        next_extd_instruction(get_next_byte().farg, cycles);
        break;
    case XOR_n:
        xor_n(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_5:
        rst_5(m_pc, m_sp, m_memory, cycles);
        break;
    case RET_P:
        ret_p(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_AF:
        pop_af(m_flag_reg, m_acc_reg, m_sp, m_memory, cycles);
        break;
    case JP_P:
        jp_p(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case DI:
        di(m_iff1, m_iff2, cycles);
        break;
    case CALL_P:
        call_p(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
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
    case RET_M:
        ret_m(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case LD_SP_HL:
        ld_sp_hl(m_sp, address_in_HL(), cycles);
        break;
    case JP_M:
        jp_m(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case EI:
        ei(m_iff1, m_iff2, cycles);
        break;
    case CALL_M:
        call_m(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case IY:
        next_ixy_instruction(get_next_byte().farg, m_iy_reg, cycles);
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
    r_tick();

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
    case SLL_B_UNDOC:
        sll_r_undoc(m_b_reg, m_flag_reg, cycles);
        break;
    case SLL_C_UNDOC:
        sll_r_undoc(m_c_reg, m_flag_reg, cycles);
        break;
    case SLL_D_UNDOC:
        sll_r_undoc(m_d_reg, m_flag_reg, cycles);
        break;
    case SLL_E_UNDOC:
        sll_r_undoc(m_e_reg, m_flag_reg, cycles);
        break;
    case SLL_H_UNDOC:
        sll_r_undoc(m_h_reg, m_flag_reg, cycles);
        break;
    case SLL_L_UNDOC:
        sll_r_undoc(m_l_reg, m_flag_reg, cycles);
        break;
    case SLL_MHL_UNDOC:
        sll_MHL_undoc(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case SLL_A_UNDOC:
        sll_r_undoc(m_acc_reg, m_flag_reg, cycles);
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

void Cpu::next_ixy_instruction(u8 ixy_opcode, u16& ixy_reg, cyc& cycles)
{
    print_debug(ixy_opcode);
    r_tick();

    switch (ixy_opcode) {
    case INC_B_UNDOC:
        inc_r_undoc(m_b_reg, m_flag_reg, cycles);
        break;
    case DEC_B_UNDOC:
        dec_r_undoc(m_b_reg, m_flag_reg, cycles);
        break;
    case LD_B_n_UNDOC:
        ld_r_n_undoc(m_b_reg, get_next_byte(), cycles);
        break;
    case ADD_IXY_BC:
        add_ixy_pp(ixy_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
        break;
    case INC_C_UNDOC:
        inc_r_undoc(m_c_reg, m_flag_reg, cycles);
        break;
    case DEC_C_UNDOC:
        dec_r_undoc(m_c_reg, m_flag_reg, cycles);
        break;
    case LD_C_n_UNDOC:
        ld_r_n_undoc(m_c_reg, get_next_byte(), cycles);
        break;
    case INC_D_UNDOC:
        inc_r_undoc(m_d_reg, m_flag_reg, cycles);
        break;
    case DEC_D_UNDOC:
        dec_r_undoc(m_d_reg, m_flag_reg, cycles);
        break;
    case LD_D_n_UNDOC:
        ld_r_n_undoc(m_d_reg, get_next_byte(), cycles);
        break;
    case ADD_IXY_DE:
        add_ixy_pp(ixy_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
        break;
    case INC_E_UNDOC:
        inc_r_undoc(m_e_reg, m_flag_reg, cycles);
        break;
    case DEC_E_UNDOC:
        dec_r_undoc(m_e_reg, m_flag_reg, cycles);
        break;
    case LD_E_n_UNDOC:
        ld_r_n_undoc(m_e_reg, get_next_byte(), cycles);
        break;
    case LD_IXY_nn:
        ld_ixy_nn(ixy_reg, get_next_word(), cycles);
        break;
    case LD_Mnn_IXY:
        ld_Mnn_ixy(ixy_reg, get_next_word(), memory(), cycles);
        break;
    case INC_IXY:
        inc_ixy(ixy_reg, cycles);
        break;
    case INC_IXH_UNDOC:
        inc_ixyh(ixy_reg, m_flag_reg, cycles);
        break;
    case DEC_IXH_UNDOC:
        dec_ixyh(ixy_reg, m_flag_reg, cycles);
        break;
    case LD_IXYH_n_UNDOC:
        ld_ixyh_n(ixy_reg, get_next_byte(), cycles);
        break;
    case ADD_IXY_IXY:
        add_ixy_pp(ixy_reg, ixy_reg, m_flag_reg, cycles);
        break;
    case LD_IXY_Mnn:
        ld_ixy_Mnn(ixy_reg, get_next_word(), memory(), cycles);
        break;
    case DEC_IXY:
        dec_ixy(ixy_reg, cycles);
        break;
    case INC_IXL_UNDOC:
        inc_ixyl(ixy_reg, m_flag_reg, cycles);
        break;
    case DEC_IXL_UNDOC:
        dec_ixyl(ixy_reg, m_flag_reg, cycles);
        break;
    case LD_IXYL_n_UNDOC:
        ld_ixyl_n(ixy_reg, get_next_byte(), cycles);
        break;
    case INC_MIXY_P_n:
        inc_MixyPd(ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case DEC_MIXY_P_n:
        dec_MixyPd(ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case LD_MIXY_P_n_d:
        ld_MixyPd_n(ixy_reg, get_next_word(), m_memory, cycles);
        break;
    case ADD_IXY_SP:
        add_ixy_pp(ixy_reg, m_sp, m_flag_reg, cycles);
        break;
    case INC_A_UNDOC:
        inc_r_undoc(m_acc_reg, m_flag_reg, cycles);
        break;
    case DEC_A_UNDOC:
        dec_r_undoc(m_acc_reg, m_flag_reg, cycles);
        break;
    case LD_A_n_UNDOC:
        ld_r_n_undoc(m_acc_reg, get_next_byte(), cycles);
        break;
    case LD_B_B_UNDOC:
        ld_r_r_undoc(m_b_reg, m_b_reg, cycles);
        break;
    case LD_B_C_UNDOC:
        ld_r_r_undoc(m_b_reg, m_c_reg, cycles);
        break;
    case LD_B_D_UNDOC:
        ld_r_r_undoc(m_b_reg, m_d_reg, cycles);
        break;
    case LD_B_E_UNDOC:
        ld_r_r_undoc(m_b_reg, m_e_reg, cycles);
        break;
    case LD_B_IXYH_UNDOC:
        ld_r_r_undoc(m_b_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_B_IXYL_UNDOC:
        ld_r_r_undoc(m_b_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_B_MIXY_P_n:
        ld_r_MixyPd(m_b_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_B_A_UNDOC:
        ld_r_r_undoc(m_b_reg, m_acc_reg, cycles);
        break;
    case LD_C_B_UNDOC:
        ld_r_r_undoc(m_c_reg, m_b_reg, cycles);
        break;
    case LD_C_C_UNDOC:
        ld_r_r_undoc(m_c_reg, m_c_reg, cycles);
        break;
    case LD_C_D_UNDOC:
        ld_r_r_undoc(m_c_reg, m_d_reg, cycles);
        break;
    case LD_C_E_UNDOC:
        ld_r_r_undoc(m_c_reg, m_e_reg, cycles);
        break;
    case LD_C_IXYH_UNDOC:
        ld_r_r_undoc(m_c_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_C_IXYL_UNDOC:
        ld_r_r_undoc(m_c_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_C_MIXY_P_n:
        ld_r_MixyPd(m_c_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_C_A_UNDOC:
        ld_r_r_undoc(m_c_reg, m_acc_reg, cycles);
        break;
    case LD_D_B_UNDOC:
        ld_r_r_undoc(m_d_reg, m_b_reg, cycles);
        break;
    case LD_D_C_UNDOC:
        ld_r_r_undoc(m_d_reg, m_c_reg, cycles);
        break;
    case LD_D_D_UNDOC:
        ld_r_r_undoc(m_d_reg, m_d_reg, cycles);
        break;
    case LD_D_E_UNDOC:
        ld_r_r_undoc(m_d_reg, m_e_reg, cycles);
        break;
    case LD_D_IXYH_UNDOC:
        ld_r_r_undoc(m_d_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_D_IXYL_UNDOC:
        ld_r_r_undoc(m_d_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_D_MIXY_P_n:
        ld_r_MixyPd(m_d_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_D_A_UNDOC:
        ld_r_r_undoc(m_d_reg, m_acc_reg, cycles);
        break;
    case LD_E_B_UNDOC:
        ld_r_r_undoc(m_e_reg, m_b_reg, cycles);
        break;
    case LD_E_C_UNDOC:
        ld_r_r_undoc(m_e_reg, m_c_reg, cycles);
        break;
    case LD_E_D_UNDOC:
        ld_r_r_undoc(m_e_reg, m_d_reg, cycles);
        break;
    case LD_E_E_UNDOC:
        ld_r_r_undoc(m_e_reg, m_e_reg, cycles);
        break;
    case LD_E_IXYH_UNDOC:
        ld_r_r_undoc(m_e_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_E_IXYL_UNDOC:
        ld_r_r_undoc(m_e_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_E_MIXY_P_n:
        ld_r_MixyPd(m_e_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_E_A_UNDOC:
        ld_r_r_undoc(m_e_reg, m_acc_reg, cycles);
        break;
    case LD_IXYH_B_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, m_b_reg, cycles);
        break;
    case LD_IXYH_C_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, m_c_reg, cycles);
        break;
    case LD_IXYH_D_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, m_d_reg, cycles);
        break;
    case LD_IXYH_E_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, m_e_reg, cycles);
        break;
    case LD_IXYH_IXYH_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_IXYH_IXYL_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_H_MIXY_P_n:
        ld_r_MixyPd(m_h_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_IXYH_A_UNDOC:
        ld_ixyh_r_undoc(ixy_reg, m_acc_reg, cycles);
        break;
    case LD_IXYL_B_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, m_b_reg, cycles);
        break;
    case LD_IXYL_C_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, m_c_reg, cycles);
        break;
    case LD_IXYL_D_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, m_d_reg, cycles);
        break;
    case LD_IXYL_E_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, m_e_reg, cycles);
        break;
    case LD_IXYL_IXYH_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_IXYL_IXYL_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_L_MIXY_P_n:
        ld_r_MixyPd(m_l_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_IXYL_A_UNDOC:
        ld_ixyl_r_undoc(ixy_reg, m_acc_reg, cycles);
        break;
    case LD_MIXY_P_n_B:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_b_reg, cycles);
        break;
    case LD_MIXY_P_n_C:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_c_reg, cycles);
        break;
    case LD_MIXY_P_n_D:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_d_reg, cycles);
        break;
    case LD_MIXY_P_n_E:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_e_reg, cycles);
        break;
    case LD_MIXY_P_n_H:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_h_reg, cycles);
        break;
    case LD_MIXY_P_n_L:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_l_reg, cycles);
        break;
    case LD_MIXY_P_n_A:
        ld_MixyPd_r(ixy_reg, get_next_byte(), m_memory, m_acc_reg, cycles);
        break;
    case LD_A_B_UNDOC:
        ld_r_r_undoc(m_acc_reg, m_b_reg, cycles);
        break;
    case LD_A_C_UNDOC:
        ld_r_r_undoc(m_acc_reg, m_c_reg, cycles);
        break;
    case LD_A_D_UNDOC:
        ld_r_r_undoc(m_acc_reg, m_d_reg, cycles);
        break;
    case LD_A_E_UNDOC:
        ld_r_r_undoc(m_acc_reg, m_e_reg, cycles);
        break;
    case LD_A_IXYH_UNDOC:
        ld_r_r_undoc(m_acc_reg, high_byte(ixy_reg), cycles);
        break;
    case LD_A_IXYL_UNDOC:
        ld_r_r_undoc(m_acc_reg, low_byte(ixy_reg), cycles);
        break;
    case LD_A_MIXY_P_n:
        ld_r_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, cycles);
        break;
    case LD_A_A_UNDOC:
        ld_r_r_undoc(m_acc_reg, m_acc_reg, cycles);
        break;
    case ADD_A_B_UNDOC:
        add_A_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADD_A_C_UNDOC:
        add_A_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADD_A_D_UNDOC:
        add_A_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADD_A_E_UNDOC:
        add_A_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADD_A_IXYH_UNDOC:
        add_A_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case ADD_A_IXYL_UNDOC:
        add_A_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case ADD_A_MIXY_P_n:
        add_A_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case ADD_A_A_UNDOC:
        add_A_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case ADC_A_B_UNDOC:
        adc_A_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADC_A_C_UNDOC:
        adc_A_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADC_A_D_UNDOC:
        adc_A_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADC_A_E_UNDOC:
        adc_A_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADC_A_MIXY_P_n:
        adc_A_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case ADC_A_IXYH_UNDOC:
        adc_A_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case ADC_A_IXYL_UNDOC:
        adc_A_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case ADC_A_A_UNDOC:
        adc_A_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case SUB_B_UNDOC:
        sub_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case SUB_C_UNDOC:
        sub_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case SUB_D_UNDOC:
        sub_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case SUB_E_UNDOC:
        sub_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case SUB_IXYH_UNDOC:
        sub_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case SUB_IXYL_UNDOC:
        sub_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case SUB_MIXY_P_n:
        sub_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case SUB_A_UNDOC:
        sub_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case SBC_A_B_UNDOC:
        sbc_A_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case SBC_A_C_UNDOC:
        sbc_A_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case SBC_A_D_UNDOC:
        sbc_A_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case SBC_A_E_UNDOC:
        sbc_A_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case SBC_A_IXYH_UNDOC:
        sbc_A_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case SBC_A_IXYL_UNDOC:
        sbc_A_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case SBC_A_MIXY_P_n:
        sbc_A_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case SBC_A_A_UNDOC:
        sbc_A_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case AND_B_UNDOC:
        and_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case AND_C_UNDOC:
        and_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case AND_D_UNDOC:
        and_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case AND_E_UNDOC:
        and_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case AND_IXYH_UNDOC:
        and_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case AND_IXYL_UNDOC:
        and_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case AND_MIXY_P_n:
        and_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case AND_A_UNDOC:
        and_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case XOR_B_UNDOC:
        xor_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case XOR_C_UNDOC:
        xor_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case XOR_D_UNDOC:
        xor_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case XOR_E_UNDOC:
        xor_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case XOR_IXYH_UNDOC:
        xor_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case XOR_IXYL_UNDOC:
        xor_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case XOR_A_UNDOC:
        xor_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case OR_B_UNDOC:
        or_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case OR_C_UNDOC:
        or_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case OR_D_UNDOC:
        or_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case OR_E_UNDOC:
        or_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case OR_IXYH_UNDOC:
        or_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case OR_IXYL_UNDOC:
        or_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case OR_MIXY_P_n:
        or_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case XOR_MIXY_P_n:
        xor_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case OR_A_UNDOC:
        or_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case CP_B_UNDOC:
        cp_r_undoc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case CP_C_UNDOC:
        cp_r_undoc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case CP_D_UNDOC:
        cp_r_undoc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case CP_E_UNDOC:
        cp_r_undoc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case CP_IXYH_UNDOC:
        cp_ixy_h_or_l(m_acc_reg, high_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case CP_IXYL_UNDOC:
        cp_ixy_h_or_l(m_acc_reg, low_byte(ixy_reg), m_flag_reg, cycles);
        break;
    case CP_MIXY_P_n:
        cp_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, m_flag_reg, cycles);
        break;
    case CP_A_UNDOC:
        cp_r_undoc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case IXY_BITS:
        next_ixy_bits_instruction(get_next_word(), ixy_reg, cycles);
        break;
    case POP_IXY:
        pop_ixy(ixy_reg, m_sp, m_memory, cycles);
        break;
    case EX_MSP_IX:
        ex_msp_ixy(m_sp, m_memory, ixy_reg, cycles);
        break;
    case PUSH_IXY:
        push_ixy(ixy_reg, m_sp, m_memory, cycles);
        break;
    case JP_MIXY:
        jp_ixy(m_pc, ixy_reg, cycles);
        break;
    case LD_SP_IXY:
        ld_sp_ixy(m_sp, ixy_reg, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(ixy_opcode, "IX/IY instructions");
    }
}

void Cpu::next_ixy_bits_instruction(NextWord args, u16& ixy_reg, cyc& cycles)
{
    u8 d = args.farg;
    u8 ixy_bits_opcode = args.sarg;
    print_debug(ixy_bits_opcode);

    switch (ixy_bits_opcode) {
    case RLC_MIXY_P_n_B_UNDOC1:
        rlc_MixyPd_r(m_b_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_C_UNDOC1:
        rlc_MixyPd_r(m_c_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_D_UNDOC1:
        rlc_MixyPd_r(m_d_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_E_UNDOC1:
        rlc_MixyPd_r(m_e_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_H_UNDOC1:
        rlc_MixyPd_r(m_h_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_L_UNDOC1:
        rlc_MixyPd_r(m_l_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n:
        rlc_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RLC_MIXY_P_n_A_UNDOC1:
        rlc_MixyPd_r(m_acc_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RRC_MIXY_P_n:
        rrc_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_B_UNDOC:
        rl_MixyPd_r(m_b_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_C_UNDOC:
        rl_MixyPd_r(m_c_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_D_UNDOC:
        rl_MixyPd_r(m_d_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_E_UNDOC:
        rl_MixyPd_r(m_e_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_H_UNDOC:
        rl_MixyPd_r(m_h_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_L_UNDOC:
        rl_MixyPd_r(m_l_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n:
        rl_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RL_MIXY_P_n_A_UNDOC:
        rl_MixyPd_r(m_acc_reg, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RR_MIXY_P_n:
        rr_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case SLA_MIXY_P_n:
        sla_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case SRA_MIXY_P_n:
        sra_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case SLL_MIXY_P_n:
        sll_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case SRL_MIXY_P_n:
        srl_MixyPd(ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_0_MIXY_P_n:
        bit_MixyPd(0, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_1_MIXY_P_n:
        bit_MixyPd(1, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_2_MIXY_P_n:
        bit_MixyPd(2, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_3_MIXY_P_n:
        bit_MixyPd(3, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_4_MIXY_P_n:
        bit_MixyPd(4, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_5_MIXY_P_n:
        bit_MixyPd(5, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_6_MIXY_P_n:
        bit_MixyPd(6, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case BIT_7_MIXY_P_n:
        bit_MixyPd(7, ixy_reg, d, m_memory, m_flag_reg, cycles);
        break;
    case RES_0_MIXY_P_n:
        res_MixyPd(0, ixy_reg, d, m_memory, cycles);
        break;
    case RES_1_MIXY_P_n:
        res_MixyPd(1, ixy_reg, d, m_memory, cycles);
        break;
    case RES_2_MIXY_P_n:
        res_MixyPd(2, ixy_reg, d, m_memory, cycles);
        break;
    case RES_3_MIXY_P_n:
        res_MixyPd(3, ixy_reg, d, m_memory, cycles);
        break;
    case RES_4_MIXY_P_n:
        res_MixyPd(4, ixy_reg, d, m_memory, cycles);
        break;
    case RES_5_MIXY_P_n:
        res_MixyPd(5, ixy_reg, d, m_memory, cycles);
        break;
    case RES_6_MIXY_P_n:
        res_MixyPd(6, ixy_reg, d, m_memory, cycles);
        break;
    case RES_7_MIXY_P_n:
        res_MixyPd(7, ixy_reg, d, m_memory, cycles);
        break;
    case SET_0_MIXY_P_n:
        set_MixyPd(0, ixy_reg, d, m_memory, cycles);
        break;
    case SET_1_MIXY_P_n:
        set_MixyPd(1, ixy_reg, d, m_memory, cycles);
        break;
    case SET_2_MIXY_P_n:
        set_MixyPd(2, ixy_reg, d, m_memory, cycles);
        break;
    case SET_3_MIXY_P_n:
        set_MixyPd(3, ixy_reg, d, m_memory, cycles);
        break;
    case SET_4_MIXY_P_n:
        set_MixyPd(4, ixy_reg, d, m_memory, cycles);
        break;
    case SET_5_MIXY_P_n:
        set_MixyPd(5, ixy_reg, d, m_memory, cycles);
        break;
    case SET_6_MIXY_P_n:
        set_MixyPd(6, ixy_reg, d, m_memory, cycles);
        break;
    case SET_7_MIXY_P_n:
        set_MixyPd(7, ixy_reg, d, m_memory, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(ixy_bits_opcode, "IX/IY bits instructions");
    }
}

void Cpu::next_extd_instruction(u8 extd_opcode, cyc& cycles)
{
    print_debug(extd_opcode);
    r_tick();

    switch (extd_opcode) {
    case IN_B_C: {
        in_r_C(m_b_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_B: {
        out_C_r(m_c_reg, m_b_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case SBC_HL_BC:
        sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
        break;
    case LD_Mnn_BC:
        ld_Mnn_dd(m_b_reg, m_c_reg, m_memory, get_next_word(), cycles);
        break;
    case NEG_UNDOC1:
    case NEG_UNDOC2:
    case NEG_UNDOC3:
    case NEG_UNDOC4:
    case NEG_UNDOC5:
    case NEG_UNDOC6:
    case NEG_UNDOC7:
    case NEG:
        neg(m_acc_reg, m_flag_reg, cycles);
        break;
    case RETN:
    case RETN_UNDOC1:
    case RETN_UNDOC2:
    case RETN_UNDOC3:
    case RETN_UNDOC4:
        retn(m_pc, m_sp, m_memory, m_iff1, m_iff2, cycles);
        break;
    case IM_0_1:
    case IM_0_2:
    case IM_0_3:
        im(m_interrupt_mode, InterruptMode::ZERO, cycles);
        break;
    case LD_I_A:
        ld_I_A(m_i_reg, m_acc_reg, cycles);
        break;
    case IN_C_C: {
        in_r_C(m_c_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_C: {
        out_C_r(m_c_reg, m_c_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case ADC_HL_BC:
        adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
        break;
    case LD_BC_Mnn:
        ld_dd_Mnn(m_b_reg, m_c_reg, get_next_word(), m_memory, cycles);
        break;
    case RETI:
        reti(m_pc, m_sp, m_memory, cycles);
        break;
    case LD_R_A:
        ld_R_A(m_r_reg, m_acc_reg, cycles);
        break;
    case IN_D_C: {
        in_r_C(m_d_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_D: {
        out_C_r(m_c_reg, m_d_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case SBC_HL_DE:
        sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
        break;
    case LD_Mnn_DE:
        ld_Mnn_dd(m_d_reg, m_e_reg, m_memory, get_next_word(), cycles);
        break;
    case IM_1_1:
    case IM_1_2:
        im(m_interrupt_mode, InterruptMode::ONE, cycles);
        break;
    case IN_A_C: {
        in_r_C(m_acc_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_A: {
        out_C_r(m_c_reg, m_acc_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case LD_A_I:
        ld_A_I(m_acc_reg, m_i_reg, m_flag_reg, m_iff2, cycles);
        break;
    case IN_E_C: {
        in_r_C(m_e_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_E: {
        out_C_r(m_c_reg, m_e_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case ADC_HL_DE:
        adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
        break;
    case LD_DE_Mnn:
        ld_dd_Mnn(m_d_reg, m_e_reg, get_next_word(), m_memory, cycles);
        break;
    case IM_2_0:
    case IM_2_1:
        im(m_interrupt_mode, InterruptMode::TWO, cycles);
        break;
    case LD_A_R:
        ld_A_R(m_acc_reg, m_r_reg, m_flag_reg, m_iff2, cycles);
        break;
    case IN_H_C: {
        in_r_C(m_h_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_H: {
        out_C_r(m_c_reg, m_h_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case SBC_HL_HL:
        sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
        break;
    case LD_Mnn_HL_UNDOC:
        ld_Mnn_dd(m_h_reg, m_l_reg, m_memory, get_next_word(), cycles);
        break;
    case RRD:
        rrd(m_acc_reg, m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case IN_L_C: {
        in_r_C(m_l_reg, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_L: {
        out_C_r(m_c_reg, m_l_reg, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case ADC_HL_HL:
        adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
        break;
    case LD_HL_Mnn_UNDOC:
        ld_dd_Mnn(m_h_reg, m_l_reg, get_next_word(), m_memory, cycles);
        break;
    case RLD:
        rld(m_acc_reg, m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case IN_C: {
        u8 throwaway;
        in_r_C(throwaway, m_c_reg, m_io_in, m_flag_reg, cycles);
        notify_in_observers(m_c_reg);
        break;
    }
    case OUT_C_0: {
        out_C_r(m_c_reg, 0, m_io_in, cycles);
        notify_out_observers(m_c_reg);
        break;
    }
    case SBC_HL_SP:
        sbc_HL_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
        break;
    case LD_Mnn_SP:
        ld_Mnn_sp(m_sp, m_memory, get_next_word(), cycles);
        break;
    case ADC_HL_SP:
        adc_hl_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
        break;
    case LD_SP_Mnn:
        ld_sp_Mnn(m_sp, m_memory, get_next_word(), cycles);
        break;
    case LDI:
        ldi(m_b_reg, m_c_reg, m_d_reg, m_e_reg, m_h_reg, m_l_reg, m_acc_reg,
            m_memory, m_flag_reg, cycles);
        break;
    case CPI:
        cpi(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_acc_reg,
            m_memory, m_flag_reg, cycles);
        break;
    case INI:
        ini(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_in, cycles);
        break;
    case OUTI:
        outi(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_out, cycles);
        break;
    case LDD:
        ldd(m_b_reg, m_c_reg, m_d_reg, m_e_reg, m_h_reg, m_l_reg, m_acc_reg,
            m_memory, m_flag_reg, cycles);
        break;
    case CPD:
        cpd(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_acc_reg, m_memory,
            m_flag_reg, cycles);
        break;
    case IND:
        ind(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_in, cycles);
        break;
    case OUTD:
        outd(m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_out, cycles);
        break;
    case LDIR:
        ldir(m_pc, m_b_reg, m_c_reg, m_d_reg, m_e_reg,
            m_h_reg, m_l_reg, m_acc_reg, m_memory, m_flag_reg, cycles);
        break;
    case CPIR:
        cpir(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg,
            m_acc_reg, m_memory, m_flag_reg, cycles);
        break;
    case INIR:
        inir(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_in, cycles);
        break;
    case OTIR:
        otir(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_out, cycles);
        break;
    case LDDR:
        lddr(m_pc, m_b_reg, m_c_reg, m_d_reg, m_e_reg, m_h_reg, m_l_reg,
            m_acc_reg, m_memory, m_flag_reg, cycles);
        break;
    case CPDR:
        cpdr(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg,
            m_acc_reg, m_memory, m_flag_reg, cycles);
        break;
    case INDR:
        indr(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_in, cycles);
        break;
    case OTDR:
        otdr(m_pc, m_b_reg, m_c_reg, m_h_reg, m_l_reg, m_memory, m_flag_reg,
            m_io_out, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(extd_opcode, "EXTD instructions");
    }
}

cyc Cpu::handle_nonmaskable_interrupt(cyc cycles)
{
    m_iff1 = false;
    m_is_nmi_interrupted = false;
    m_is_halted = false;

    r_tick();

    nmi(m_pc, m_sp, m_memory, cycles);

    return cycles;
}

cyc Cpu::handle_maskable_interrupt_0(cyc cycles)
{
    m_iff1 = m_iff2 = false;
    m_is_interrupted = false;
    m_is_halted = false;

    r_tick();

    m_opcode = m_instruction_from_interruptor;
    cycles = 13;

    return cycles;
}

cyc Cpu::handle_maskable_interrupt_1_2(cyc cycles)
{
    m_iff1 = m_iff2 = false;
    m_is_interrupted = false;
    m_is_halted = false;

    r_tick();

    switch (m_interrupt_mode) {
    case InterruptMode::ZERO:
        throw std::invalid_argument("Programming error: Should handle this mode somewhere else.");
    case InterruptMode::ONE:
        rst_7(m_pc, m_sp, m_memory, cycles);
        cycles = 13;
        break;
    case InterruptMode::TWO:
        const u16 address_of_interrupt_vector = to_u16(m_i_reg, m_instruction_from_interruptor);
        const u8 farg = m_memory.read(address_of_interrupt_vector);
        const u8 sarg = m_memory.read(address_of_interrupt_vector + 1);
        call(m_pc, m_sp, m_memory, { .farg = farg, .sarg = sarg }, cycles);
        cycles = 19;
        break;
    }

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

u8 Cpu::a_p() const
{
    return m_acc_p_reg;
}

u8 Cpu::b() const
{
    return m_b_reg;
}

u8 Cpu::b_p() const
{
    return m_b_p_reg;
}

u8 Cpu::c() const
{
    return m_c_reg;
}

u8 Cpu::c_p() const
{
    return m_c_p_reg;
}

u8 Cpu::d() const
{
    return m_d_reg;
}

u8 Cpu::d_p() const
{
    return m_d_p_reg;
}

u8 Cpu::e() const
{
    return m_e_reg;
}

u8 Cpu::e_p() const
{
    return m_e_p_reg;
}

u8 Cpu::h() const
{
    return m_h_reg;
}

u8 Cpu::h_p() const
{
    return m_h_p_reg;
}

u8 Cpu::l() const
{
    return m_l_reg;
}

u8 Cpu::l_p() const
{
    return m_l_p_reg;
}

u8 Cpu::f() const
{
    return m_flag_reg.to_u8();
}

u8 Cpu::f_p() const
{
    return m_flag_p_reg.to_u8();
}

u16 Cpu::ix() const
{
    return m_ix_reg;
}

u16 Cpu::iy() const
{
    return m_iy_reg;
}

u8 Cpu::i() const
{
    return m_i_reg;
}

u8 Cpu::r() const
{
    return m_r_reg;
}

bool Cpu::is_interrupted() const
{
    return m_is_interrupted;
}

InterruptMode Cpu::interrupt_mode() const
{
    return m_interrupt_mode;
}

void Cpu::notify_out_observers(u8 port)
{
    for (OutObserver* observer : m_out_observers) {
        observer->out_changed(port);
    }
}

void Cpu::notify_in_observers(u8 port)
{
    for (InObserver* observer : m_in_observers) {
        observer->in_requested(port);
    }
}

void Cpu::r_tick()
{
    m_r_reg = m_r_reg == INT8_MAX ? 0 : m_r_reg + 1;
}

void Cpu::print_debug(u8 opcode)
{
    if (false) {
        std::cout << "pc=" << hexify(m_pc)
                  << ",sp=" << hexify(m_sp)
                  << ",op=" << hexify(opcode)
                  << ",a=" << hexify(m_acc_reg)
                  << ",b=" << hexify(m_b_reg)
                  << ",c=" << hexify(m_c_reg)
                  << ",d=" << hexify(m_d_reg)
                  << ",e=" << hexify(m_e_reg)
                  << ",h=" << hexify(m_h_reg)
                  << ",l=" << hexify(m_l_reg)
                  << ",a'=" << hexify(m_acc_p_reg)
                  << ",b'=" << hexify(m_b_p_reg)
                  << ",c'=" << hexify(m_c_p_reg)
                  << ",d'=" << hexify(m_d_p_reg)
                  << ",e'=" << hexify(m_e_p_reg)
                  << ",h'=" << hexify(m_h_p_reg)
                  << ",l'=" << hexify(m_l_p_reg)
                  << ",ix=" << hexify(m_ix_reg)
                  << ",iy=" << hexify(m_iy_reg)
                  << ",i=" << hexify(m_i_reg)
                  << ",r=" << hexify(m_r_reg)
                  << ",c=" << m_flag_reg.is_carry_flag_set()
                  << ",po=" << m_flag_reg.is_parity_overflow_flag_set()
                  << ",hc=" << m_flag_reg.is_half_carry_flag_set()
                  << ",n=" << m_flag_reg.is_add_subtract_flag_set()
                  << ",z=" << m_flag_reg.is_zero_flag_set()
                  << ",s=" << m_flag_reg.is_sign_flag_set()
                  << ",y=" << m_flag_reg.is_y_flag_set()
                  << ",x=" << m_flag_reg.is_x_flag_set()
                  //                  << ",c'=" << m_flag_p_reg.is_carry_flag_set()
                  //                  << ",po'=" << m_flag_p_reg.is_parity_overflow_flag_set()
                  //                  << ",hc'=" << m_flag_p_reg.is_half_carry_flag_set()
                  //                  << ",n'=" << m_flag_p_reg.is_add_subtract_flag_set()
                  //                  << ",z'=" << m_flag_p_reg.is_zero_flag_set()
                  //                  << ",s'=" << m_flag_p_reg.is_sign_flag_set()
                  << "\n"
                  << std::flush;
    }
}
}
