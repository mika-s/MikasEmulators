#include "cpu.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace emu::i8080 {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::byte::to_u16;
using emu::util::string::hexify;

Cpu::Cpu(
    EmulatorMemory<u16, u8>& memory,
    const u16 initial_pc)
    : m_is_halted(false)
    , m_inte(false)
    , m_is_interrupted(false)
    , m_instruction_from_interruptor(0)
    , m_memory(memory)
    , m_memory_size(memory.size())
    , m_io_in(number_of_io_ports)
    , m_io_out(number_of_io_ports)
    , m_opcode(0)
    , m_sp(0)
    , m_pc(initial_pc)
    , m_acc_reg(0)
    , m_b_reg(0)
    , m_c_reg(0)
    , m_d_reg(0)
    , m_e_reg(0)
    , m_h_reg(0)
    , m_l_reg(0)
{
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
    m_b_reg = 0;
    m_c_reg = 0;
    m_d_reg = 0;
    m_e_reg = 0;
    m_h_reg = 0;
    m_l_reg = 0;
    m_acc_reg = 0;
    m_flag_reg.reset();
    m_pc = 0;
    m_sp = 0;
    m_is_halted = false;
    m_inte = false;
    m_is_interrupted = false;
    m_instruction_from_interruptor = 0;
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

bool Cpu::is_inta() const
{
    return m_inte;
}

void Cpu::input(u8 port, u8 value)
{
    m_io_in[port] = value;
}

cyc Cpu::next_instruction()
{
    cyc cycles = 0;

    if (m_inte && m_is_interrupted) {
        m_inte = false;
        m_is_interrupted = false;
        m_is_halted = false;
        m_opcode = m_instruction_from_interruptor;
    } else if (m_is_halted) {
        return 4; // TODO: What is the proper value while NOPing during halt?
    } else {
        m_opcode = get_next_byte().farg;
    }

    switch (m_opcode) {
    case NOP:
        nop(cycles);
        break;
    case LXI_B:
        lxi(m_b_reg, m_c_reg, get_next_word(), cycles);
        break;
    case STAX_B:
        stax(m_acc_reg, m_b_reg, m_c_reg, m_memory, cycles);
        break;
    case INX_B:
        inx(m_b_reg, m_c_reg, cycles);
        break;
    case INR_B:
        inr_r(m_b_reg, m_flag_reg, cycles);
        break;
    case DCR_B:
        dcr_r(m_b_reg, m_flag_reg, cycles);
        break;
    case MVI_B:
        mvi_r(m_b_reg, get_next_byte(), cycles);
        break;
    case RLC_B:
        rlc(m_acc_reg, m_flag_reg, cycles);
        break;
    case UNUSED_NOP_1:
        unused_1(m_opcode, cycles);
        break;
    case DAD_B:
        dad(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
        break;
    case LDAX_B:
        ldax(m_acc_reg, m_b_reg, m_c_reg, m_memory, cycles);
        break;
    case DCX_B:
        dcx(m_b_reg, m_c_reg, cycles);
        break;
    case INR_C:
        inr_r(m_c_reg, m_flag_reg, cycles);
        break;
    case DCR_C:
        dcr_r(m_c_reg, m_flag_reg, cycles);
        break;
    case MVI_C:
        mvi_r(m_c_reg, get_next_byte(), cycles);
        break;
    case RRC:
        rrc(m_acc_reg, m_flag_reg, cycles);
        break;
    case UNUSED_NOP_2:
        unused_1(m_opcode, cycles);
        break;
    case LXI_D:
        lxi(m_d_reg, m_e_reg, get_next_word(), cycles);
        break;
    case STAX_D:
        stax(m_acc_reg, m_d_reg, m_e_reg, m_memory, cycles);
        break;
    case INX_D:
        inx(m_d_reg, m_e_reg, cycles);
        break;
    case INR_D:
        inr_r(m_d_reg, m_flag_reg, cycles);
        break;
    case DCR_D:
        dcr_r(m_d_reg, m_flag_reg, cycles);
        break;
    case MVI_D:
        mvi_r(m_d_reg, get_next_byte(), cycles);
        break;
    case RAL:
        ral(m_acc_reg, m_flag_reg, cycles);
        break;
    case UNUSED_NOP_3:
        unused_1(m_opcode, cycles);
        break;
    case DAD_D:
        dad(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
        break;
    case LDAX_D:
        ldax(m_acc_reg, m_d_reg, m_e_reg, m_memory, cycles);
        break;
    case DCX_D:
        dcx(m_d_reg, m_e_reg, cycles);
        break;
    case INR_E:
        inr_r(m_e_reg, m_flag_reg, cycles);
        break;
    case DCR_E:
        dcr_r(m_e_reg, m_flag_reg, cycles);
        break;
    case MVI_E:
        mvi_r(m_e_reg, get_next_byte(), cycles);
        break;
    case RAR:
        rar(m_acc_reg, m_flag_reg, cycles);
        break;
    case UNUSED_NOP_4:
        unused_1(m_opcode, cycles);
        break;
    case LXI_H:
        lxi(m_h_reg, m_l_reg, get_next_word(), cycles);
        break;
    case SHLD:
        shld(m_l_reg, m_h_reg, m_memory, get_next_word(), cycles);
        break;
    case INX_H:
        inx(m_h_reg, m_l_reg, cycles);
        break;
    case INR_H:
        inr_r(m_h_reg, m_flag_reg, cycles);
        break;
    case DCR_H:
        dcr_r(m_h_reg, m_flag_reg, cycles);
        break;
    case MVI_H:
        mvi_r(m_h_reg, get_next_byte(), cycles);
        break;
    case DAA:
        daa(m_acc_reg, m_flag_reg, cycles);
        break;
    case UNUSED_NOP_5:
        unused_1(m_opcode, cycles);
        break;
    case DAD_H:
        dad(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
        break;
    case LHLD:
        lhld(m_l_reg, m_h_reg, m_memory, get_next_word(), cycles);
        break;
    case DCX_H:
        dcx(m_h_reg, m_l_reg, cycles);
        break;
    case INR_L:
        inr_r(m_l_reg, m_flag_reg, cycles);
        break;
    case DCR_L:
        dcr_r(m_l_reg, m_flag_reg, cycles);
        break;
    case MVI_L:
        mvi_r(m_l_reg, get_next_byte(), cycles);
        break;
    case CMA:
        cma(m_acc_reg, cycles);
        break;
    case UNUSED_NOP_6:
        unused_1(m_opcode, cycles);
        break;
    case LXI_SP:
        lxi_sp(m_sp, get_next_word(), cycles);
        break;
    case STA:
        sta(m_acc_reg, m_memory, get_next_word(), cycles);
        break;
    case INX_SP:
        inx_sp(m_sp, cycles);
        break;
    case INR_M:
        inr_m(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case DCR_M:
        dcr_m(m_memory, address_in_HL(), m_flag_reg, cycles);
        break;
    case MVI_M:
        mvi_m(m_memory, address_in_HL(), get_next_byte(), cycles);
        break;
    case STC:
        stc(m_flag_reg, cycles);
        break;
    case UNUSED_NOP_7:
        unused_1(m_opcode, cycles);
        break;
    case DAD_SP:
        dad(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
        break;
    case LDA:
        lda(m_acc_reg, m_memory, get_next_word(), cycles);
        break;
    case DCX_SP:
        dcx_sp(m_sp, cycles);
        break;
    case INR_A:
        inr_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case DCR_A:
        dcr_r(m_acc_reg, m_flag_reg, cycles);
        break;
    case MVI_A:
        mvi_r(m_acc_reg, get_next_byte(), cycles);
        break;
    case CMC:
        cmc(m_flag_reg, cycles);
        break;
    case MOV_B_B:
        mov_r_r(m_b_reg, m_b_reg, cycles);
        break;
    case MOV_B_C:
        mov_r_r(m_b_reg, m_c_reg, cycles);
        break;
    case MOV_B_D:
        mov_r_r(m_b_reg, m_d_reg, cycles);
        break;
    case MOV_B_E:
        mov_r_r(m_b_reg, m_e_reg, cycles);
        break;
    case MOV_B_H:
        mov_r_r(m_b_reg, m_h_reg, cycles);
        break;
    case MOV_B_L:
        mov_r_r(m_b_reg, m_l_reg, cycles);
        break;
    case MOV_B_M:
        mov_r_m(m_b_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_B_A:
        mov_r_r(m_b_reg, m_acc_reg, cycles);
        break;
    case MOV_C_B:
        mov_r_r(m_c_reg, m_b_reg, cycles);
        break;
    case MOV_C_C:
        mov_r_r(m_c_reg, m_c_reg, cycles);
        break;
    case MOV_C_D:
        mov_r_r(m_c_reg, m_d_reg, cycles);
        break;
    case MOV_C_E:
        mov_r_r(m_c_reg, m_e_reg, cycles);
        break;
    case MOV_C_H:
        mov_r_r(m_c_reg, m_h_reg, cycles);
        break;
    case MOV_C_L:
        mov_r_r(m_c_reg, m_l_reg, cycles);
        break;
    case MOV_C_M:
        mov_r_m(m_c_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_C_A:
        mov_r_r(m_c_reg, m_acc_reg, cycles);
        break;
    case MOV_D_B:
        mov_r_r(m_d_reg, m_b_reg, cycles);
        break;
    case MOV_D_C:
        mov_r_r(m_d_reg, m_c_reg, cycles);
        break;
    case MOV_D_D:
        mov_r_r(m_d_reg, m_d_reg, cycles);
        break;
    case MOV_D_E:
        mov_r_r(m_d_reg, m_e_reg, cycles);
        break;
    case MOV_D_H:
        mov_r_r(m_d_reg, m_h_reg, cycles);
        break;
    case MOV_D_L:
        mov_r_r(m_d_reg, m_l_reg, cycles);
        break;
    case MOV_D_M:
        mov_r_m(m_d_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_D_A:
        mov_r_r(m_d_reg, m_acc_reg, cycles);
        break;
    case MOV_E_B:
        mov_r_r(m_e_reg, m_b_reg, cycles);
        break;
    case MOV_E_C:
        mov_r_r(m_e_reg, m_c_reg, cycles);
        break;
    case MOV_E_D:
        mov_r_r(m_e_reg, m_d_reg, cycles);
        break;
    case MOV_E_E:
        mov_r_r(m_e_reg, m_e_reg, cycles);
        break;
    case MOV_E_H:
        mov_r_r(m_e_reg, m_h_reg, cycles);
        break;
    case MOV_E_L:
        mov_r_r(m_e_reg, m_l_reg, cycles);
        break;
    case MOV_E_M:
        mov_r_m(m_e_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_E_A:
        mov_r_r(m_e_reg, m_acc_reg, cycles);
        break;
    case MOV_H_B:
        mov_r_r(m_h_reg, m_b_reg, cycles);
        break;
    case MOV_H_C:
        mov_r_r(m_h_reg, m_c_reg, cycles);
        break;
    case MOV_H_D:
        mov_r_r(m_h_reg, m_d_reg, cycles);
        break;
    case MOV_H_E:
        mov_r_r(m_h_reg, m_e_reg, cycles);
        break;
    case MOV_H_H:
        mov_r_r(m_h_reg, m_h_reg, cycles);
        break;
    case MOV_H_L:
        mov_r_r(m_h_reg, m_l_reg, cycles);
        break;
    case MOV_H_M:
        mov_r_m(m_h_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_H_A:
        mov_r_r(m_h_reg, m_acc_reg, cycles);
        break;
    case MOV_L_B:
        mov_r_r(m_l_reg, m_b_reg, cycles);
        break;
    case MOV_L_C:
        mov_r_r(m_l_reg, m_c_reg, cycles);
        break;
    case MOV_L_D:
        mov_r_r(m_l_reg, m_d_reg, cycles);
        break;
    case MOV_L_E:
        mov_r_r(m_l_reg, m_e_reg, cycles);
        break;
    case MOV_L_H:
        mov_r_r(m_l_reg, m_h_reg, cycles);
        break;
    case MOV_L_L:
        mov_r_r(m_l_reg, m_l_reg, cycles);
        break;
    case MOV_L_M:
        mov_r_m(m_l_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_L_A:
        mov_r_r(m_l_reg, m_acc_reg, cycles);
        break;
    case MOV_M_B:
        mov_m_r(m_memory, address_in_HL(), m_b_reg, cycles);
        break;
    case MOV_M_C:
        mov_m_r(m_memory, address_in_HL(), m_c_reg, cycles);
        break;
    case MOV_M_D:
        mov_m_r(m_memory, address_in_HL(), m_d_reg, cycles);
        break;
    case MOV_M_E:
        mov_m_r(m_memory, address_in_HL(), m_e_reg, cycles);
        break;
    case MOV_M_H:
        mov_m_r(m_memory, address_in_HL(), m_h_reg, cycles);
        break;
    case MOV_M_L:
        mov_m_r(m_memory, address_in_HL(), m_l_reg, cycles);
        break;
    case HLT:
        hlt(m_is_halted, cycles);
        break;
    case MOV_M_A:
        mov_m_r(m_memory, address_in_HL(), m_acc_reg, cycles);
        break;
    case MOV_A_B:
        mov_r_r(m_acc_reg, m_b_reg, cycles);
        break;
    case MOV_A_C:
        mov_r_r(m_acc_reg, m_c_reg, cycles);
        break;
    case MOV_A_D:
        mov_r_r(m_acc_reg, m_d_reg, cycles);
        break;
    case MOV_A_E:
        mov_r_r(m_acc_reg, m_e_reg, cycles);
        break;
    case MOV_A_H:
        mov_r_r(m_acc_reg, m_h_reg, cycles);
        break;
    case MOV_A_L:
        mov_r_r(m_acc_reg, m_l_reg, cycles);
        break;
    case MOV_A_M:
        mov_r_m(m_acc_reg, m_memory.read(address_in_HL()), cycles);
        break;
    case MOV_A_A:
        mov_r_r(m_acc_reg, m_acc_reg, cycles);
        break;
    case ADD_B:
        add_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADD_C:
        add_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADD_D:
        add_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADD_E:
        add_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADD_H:
        add_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ADD_L:
        add_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ADD_M:
        add_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ADD_A:
        add_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case ADC_B:
        adc_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ADC_C:
        adc_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ADC_D:
        adc_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ADC_E:
        adc_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ADC_H:
        adc_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ADC_L:
        adc_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ADC_M:
        adc_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ADC_A:
        adc_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
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
    case SUB_M:
        sub_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case SUB_A:
        sub_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case SBB_B:
        sbb_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case SBB_C:
        sbb_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case SBB_D:
        sbb_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case SBB_E:
        sbb_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case SBB_H:
        sbb_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case SBB_L:
        sbb_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case SBB_M:
        sbb_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case SBB_A:
        sbb_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case ANA_B:
        ana_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ANA_C:
        ana_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ANA_D:
        ana_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ANA_E:
        ana_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ANA_H:
        ana_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ANA_L:
        ana_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ANA_M:
        ana_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ANA_A:
        ana_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case XRA_B:
        xra_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case XRA_C:
        xra_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case XRA_D:
        xra_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case XRA_E:
        xra_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case XRA_H:
        xra_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case XRA_L:
        xra_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case XRA_M:
        xra_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case XRA_A:
        xra_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case ORA_B:
        ora_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case ORA_C:
        ora_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case ORA_D:
        ora_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case ORA_E:
        ora_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case ORA_H:
        ora_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case ORA_L:
        ora_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case ORA_M:
        ora_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case ORA_A:
        ora_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case CMP_B:
        cmp_r(m_acc_reg, m_b_reg, m_flag_reg, cycles);
        break;
    case CMP_C:
        cmp_r(m_acc_reg, m_c_reg, m_flag_reg, cycles);
        break;
    case CMP_D:
        cmp_r(m_acc_reg, m_d_reg, m_flag_reg, cycles);
        break;
    case CMP_E:
        cmp_r(m_acc_reg, m_e_reg, m_flag_reg, cycles);
        break;
    case CMP_H:
        cmp_r(m_acc_reg, m_h_reg, m_flag_reg, cycles);
        break;
    case CMP_L:
        cmp_r(m_acc_reg, m_l_reg, m_flag_reg, cycles);
        break;
    case CMP_M:
        cmp_m(m_acc_reg, m_memory.read(address_in_HL()), m_flag_reg, cycles);
        break;
    case CMP_A:
        cmp_r(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
        break;
    case RNZ:
        rnz(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_B:
        pop(m_b_reg, m_c_reg, m_sp, m_memory, cycles);
        break;
    case JNZ:
        jnz(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case JMP:
        jmp(m_pc, get_next_word(), cycles);
        break;
    case CNZ:
        cnz(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_B:
        push(m_b_reg, m_c_reg, m_sp, m_memory, cycles);
        break;
    case ADI:
        adi(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_0:
        rst_0(m_pc, m_sp, m_memory, cycles);
        break;
    case RZ:
        rz(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case RET:
        ret(m_pc, m_sp, m_memory, cycles);
        break;
    case JZ:
        jz(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case UNUSED_JMP_1:
        unused_3(m_opcode, m_pc, cycles);
        break;
    case CZ:
        cz(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case CALL:
        call(m_pc, m_sp, m_memory, get_next_word(), cycles);
        break;
    case ACI:
        aci(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_1:
        rst_1(m_pc, m_sp, m_memory, cycles);
        break;
    case RNC:
        rnc(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_D:
        pop(m_d_reg, m_e_reg, m_sp, m_memory, cycles);
        break;
    case JNC:
        jnc(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case OUT: {
        NextByte args = get_next_byte();
        out(m_acc_reg, args, m_io_out, cycles);
        notify_out_observers(args.farg);
        break;
    }
    case CNC:
        cnc(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_D:
        push(m_d_reg, m_e_reg, m_sp, m_memory, cycles);
        break;
    case SUI:
        sui(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_2:
        rst_2(m_pc, m_sp, m_memory, cycles);
        break;
    case RC:
        rc(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case UNUSED_RET_1:
        unused_1(m_opcode, cycles);
        break;
    case JC:
        jc(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case IN: {
        NextByte args = get_next_byte();
        notify_in_observers(args.farg);
        in(m_acc_reg, args, m_io_in, cycles);
        break;
    }
    case CC:
        cc(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case UNUSED_CALL_1:
        unused_3(m_opcode, m_pc, cycles);
        break;
    case SBI:
        sbi(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_3:
        rst_3(m_pc, m_sp, m_memory, cycles);
        break;
    case RPO:
        rpo(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_H:
        pop(m_h_reg, m_l_reg, m_sp, m_memory, cycles);
        break;
    case JPO:
        jpo(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case XTHL:
        xthl(m_sp, m_memory, m_h_reg, m_l_reg, cycles);
        break;
    case CPO:
        cpo(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_H:
        push(m_h_reg, m_l_reg, m_sp, m_memory, cycles);
        break;
    case ANI:
        ani(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_4:
        rst_4(m_pc, m_sp, m_memory, cycles);
        break;
    case RPE:
        rpe(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case PCHL:
        pchl(m_pc, address_in_HL(), cycles);
        break;
    case JPE:
        jpe(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case XCHG:
        xchg(m_h_reg, m_l_reg, m_d_reg, m_e_reg, cycles);
        break;
    case CPE:
        cpe(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case UNUSED_CALL_2:
        unused_3(m_opcode, m_pc, cycles);
        break;
    case XRI:
        xri(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_5:
        rst_5(m_pc, m_sp, m_memory, cycles);
        break;
    case RP:
        rp(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case POP_PSW:
        pop_psw(m_flag_reg, m_acc_reg, m_sp, m_memory, cycles);
        break;
    case JP:
        jp(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case DI:
        di(m_inte, cycles);
        break;
    case CP:
        cp(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case PUSH_PSW:
        push_psw(m_flag_reg, m_acc_reg, m_sp, m_memory, cycles);
        break;
    case ORI:
        ori(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_6:
        rst_6(m_pc, m_sp, m_memory, cycles);
        break;
    case RM:
        rm(m_pc, m_sp, m_memory, m_flag_reg, cycles);
        break;
    case SPHL:
        sphl(m_sp, address_in_HL(), cycles);
        break;
    case JM:
        jm(m_pc, get_next_word(), m_flag_reg, cycles);
        break;
    case EI:
        ei(m_inte, cycles);
        break;
    case CM:
        cm(m_pc, m_sp, m_memory, get_next_word(), m_flag_reg, cycles);
        break;
    case UNUSED_CALL_3:
        unused_3(m_opcode, m_pc, cycles);
        break;
    case CPI:
        cpi(m_acc_reg, get_next_byte(), m_flag_reg, cycles);
        break;
    case RST_7:
        rst_7(m_pc, m_sp, m_memory, cycles);
        break;
    default:
        throw UnrecognizedOpcodeException(m_opcode);
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

bool Cpu::is_interrupted() const
{
    return m_is_interrupted;
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

void Cpu::print_debug()
{
    std::cout << "pc=" << hexify(m_pc)
              << ",sp=" << hexify(m_sp)
              << ",op=" << hexify(m_opcode)
              << ",a=" << hexify(m_acc_reg)
              << ",b=" << hexify(m_b_reg)
              << ",c=" << hexify(m_c_reg)
              << ",d=" << hexify(m_d_reg)
              << ",e=" << hexify(m_e_reg)
              << ",h=" << hexify(m_h_reg)
              << ",l=" << hexify(m_l_reg)
              << ",ca=" << m_flag_reg.is_carry_flag_set()
              << ",pa=" << m_flag_reg.is_parity_flag_set()
              << ",ac=" << m_flag_reg.is_aux_carry_flag_set()
              << ",z=" << m_flag_reg.is_zero_flag_set()
              << ",s=" << m_flag_reg.is_sign_flag_set()
              << "\n";
}
}
