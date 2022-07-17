#include <algorithm>
#include <iostream>
#include "cpu.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::util::byte::to_u16;
    using emu::util::string::hexify;
    using emu::exceptions::UnrecognizedOpcodeException;

    Cpu::Cpu(
            EmulatorMemory &memory,
            const u16 initial_pc
    ) :
            m_is_stopped(true),
            m_inte(false),
            m_is_interrupted(false),
            m_instruction_from_interruptor(0),
            m_memory(memory),
            m_memory_size(memory.size()),
            m_opcode(0),
            m_sp(0),
            m_pc(initial_pc),
            m_acc_reg(0),
            m_b_reg(0),
            m_c_reg(0),
            m_d_reg(0),
            m_e_reg(0),
            m_h_reg(0),
            m_l_reg(0) {
        m_io_in.reserve(number_of_io_ports);
        m_io_out.reserve(number_of_io_ports);
        for (unsigned int i = 0; i < number_of_io_ports; ++i) {
            m_io_in.push_back(0);
            m_io_out.push_back(0);
        }
    }

    Cpu::~Cpu() {
        for (auto observer: m_out_observers) {
            remove_out_observer(observer);
        }

        for (auto observer: m_in_observers) {
            remove_in_observer(observer);
        }
    }

    void Cpu::add_out_observer(OutObserver &observer) {
        m_out_observers.push_back(&observer);
    }

    void Cpu::remove_out_observer(OutObserver *observer) {
        m_out_observers.erase(
                std::remove(m_out_observers.begin(), m_out_observers.end(), observer),
                m_out_observers.end()
        );
    }

    void Cpu::add_in_observer(InObserver &observer) {
        m_in_observers.push_back(&observer);
    }

    void Cpu::remove_in_observer(InObserver *observer) {
        m_in_observers.erase(
                std::remove(m_in_observers.begin(), m_in_observers.end(), observer),
                m_in_observers.end()
        );
    }

    bool Cpu::can_run_next_instruction() const {
        return m_pc < m_memory_size && !m_is_stopped;
    }

    void Cpu::reset_state() {
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
        m_is_stopped = true;
        m_inte = false;
        m_is_interrupted = false;
        m_instruction_from_interruptor = 0;
    }

    void Cpu::start() {
        m_is_stopped = false;
    }

    void Cpu::stop() {
        m_is_stopped = true;
    }

    void Cpu::interrupt(u8 instruction_to_perform) {
        m_is_interrupted = true;
        m_instruction_from_interruptor = instruction_to_perform;
    }

    bool Cpu::is_inta() const {
        return m_inte;
    }

    void Cpu::input(int port, u8 value) {
        m_io_in[port] = value;
    }

    unsigned long Cpu::next_instruction() {
        unsigned long cycles = 0;

        if (m_inte && m_is_interrupted) {
            m_inte = false;
            m_is_interrupted = false;
            m_opcode = m_instruction_from_interruptor;
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
                inr(m_b_reg, m_flag_reg, cycles);
                break;
            case DCR_B:
                dcr(m_b_reg, m_flag_reg, cycles);
                break;
            case MVI_B:
                mvi(m_b_reg, get_next_byte(), cycles);
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
                inr(m_c_reg, m_flag_reg, cycles);
                break;
            case DCR_C:
                dcr(m_c_reg, m_flag_reg, cycles);
                break;
            case MVI_C:
                mvi(m_c_reg, get_next_byte(), cycles);
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
                inr(m_d_reg, m_flag_reg, cycles);
                break;
            case DCR_D:
                dcr(m_d_reg, m_flag_reg, cycles);
                break;
            case MVI_D:
                mvi(m_d_reg, get_next_byte(), cycles);
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
                inr(m_e_reg, m_flag_reg, cycles);
                break;
            case DCR_E:
                dcr(m_e_reg, m_flag_reg, cycles);
                break;
            case MVI_E:
                mvi(m_e_reg, get_next_byte(), cycles);
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
                inr(m_h_reg, m_flag_reg, cycles);
                break;
            case DCR_H:
                dcr(m_h_reg, m_flag_reg, cycles);
                break;
            case MVI_H:
                mvi(m_h_reg, get_next_byte(), cycles);
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
                inr(m_l_reg, m_flag_reg, cycles);
                break;
            case DCR_L:
                dcr(m_l_reg, m_flag_reg, cycles);
                break;
            case MVI_L:
                mvi(m_l_reg, get_next_byte(), cycles);
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
                inr(m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case DCR_M:
                dcr(m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case MVI_M:
                mvi(m_memory[address_in_HL()], get_next_byte(), cycles, true);
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
                inr(m_acc_reg, m_flag_reg, cycles);
                break;
            case DCR_A:
                dcr(m_acc_reg, m_flag_reg, cycles);
                break;
            case MVI_A:
                mvi(m_acc_reg, get_next_byte(), cycles);
                break;
            case CMC:
                cmc(m_flag_reg, cycles);
                break;
            case MOV_B_B:
                mov(m_b_reg, m_b_reg, cycles);
                break;
            case MOV_B_C:
                mov(m_b_reg, m_c_reg, cycles);
                break;
            case MOV_B_D:
                mov(m_b_reg, m_d_reg, cycles);
                break;
            case MOV_B_E:
                mov(m_b_reg, m_e_reg, cycles);
                break;
            case MOV_B_H:
                mov(m_b_reg, m_h_reg, cycles);
                break;
            case MOV_B_L:
                mov(m_b_reg, m_l_reg, cycles);
                break;
            case MOV_B_M:
                mov(m_b_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_B_A:
                mov(m_b_reg, m_acc_reg, cycles);
                break;
            case MOV_C_B:
                mov(m_c_reg, m_b_reg, cycles);
                break;
            case MOV_C_C:
                mov(m_c_reg, m_c_reg, cycles);
                break;
            case MOV_C_D:
                mov(m_c_reg, m_d_reg, cycles);
                break;
            case MOV_C_E:
                mov(m_c_reg, m_e_reg, cycles);
                break;
            case MOV_C_H:
                mov(m_c_reg, m_h_reg, cycles);
                break;
            case MOV_C_L:
                mov(m_c_reg, m_l_reg, cycles);
                break;
            case MOV_C_M:
                mov(m_c_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_C_A:
                mov(m_c_reg, m_acc_reg, cycles);
                break;
            case MOV_D_B:
                mov(m_d_reg, m_b_reg, cycles);
                break;
            case MOV_D_C:
                mov(m_d_reg, m_c_reg, cycles);
                break;
            case MOV_D_D:
                mov(m_d_reg, m_d_reg, cycles);
                break;
            case MOV_D_E:
                mov(m_d_reg, m_e_reg, cycles);
                break;
            case MOV_D_H:
                mov(m_d_reg, m_h_reg, cycles);
                break;
            case MOV_D_L:
                mov(m_d_reg, m_l_reg, cycles);
                break;
            case MOV_D_M:
                mov(m_d_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_D_A:
                mov(m_d_reg, m_acc_reg, cycles);
                break;
            case MOV_E_B:
                mov(m_e_reg, m_b_reg, cycles);
                break;
            case MOV_E_C:
                mov(m_e_reg, m_c_reg, cycles);
                break;
            case MOV_E_D:
                mov(m_e_reg, m_d_reg, cycles);
                break;
            case MOV_E_E:
                mov(m_e_reg, m_e_reg, cycles);
                break;
            case MOV_E_H:
                mov(m_e_reg, m_h_reg, cycles);
                break;
            case MOV_E_L:
                mov(m_e_reg, m_l_reg, cycles);
                break;
            case MOV_E_M:
                mov(m_e_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_E_A:
                mov(m_e_reg, m_acc_reg, cycles);
                break;
            case MOV_H_B:
                mov(m_h_reg, m_b_reg, cycles);
                break;
            case MOV_H_C:
                mov(m_h_reg, m_c_reg, cycles);
                break;
            case MOV_H_D:
                mov(m_h_reg, m_d_reg, cycles);
                break;
            case MOV_H_E:
                mov(m_h_reg, m_e_reg, cycles);
                break;
            case MOV_H_H:
                mov(m_h_reg, m_h_reg, cycles);
                break;
            case MOV_H_L:
                mov(m_h_reg, m_l_reg, cycles);
                break;
            case MOV_H_M:
                mov(m_h_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_H_A:
                mov(m_h_reg, m_acc_reg, cycles);
                break;
            case MOV_L_B:
                mov(m_l_reg, m_b_reg, cycles);
                break;
            case MOV_L_C:
                mov(m_l_reg, m_c_reg, cycles);
                break;
            case MOV_L_D:
                mov(m_l_reg, m_d_reg, cycles);
                break;
            case MOV_L_E:
                mov(m_l_reg, m_e_reg, cycles);
                break;
            case MOV_L_H:
                mov(m_l_reg, m_h_reg, cycles);
                break;
            case MOV_L_L:
                mov(m_l_reg, m_l_reg, cycles);
                break;
            case MOV_L_M:
                mov(m_l_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_L_A:
                mov(m_l_reg, m_acc_reg, cycles);
                break;
            case MOV_M_B:
                mov(m_memory[address_in_HL()], m_b_reg, cycles, true);
                break;
            case MOV_M_C:
                mov(m_memory[address_in_HL()], m_c_reg, cycles, true);
                break;
            case MOV_M_D:
                mov(m_memory[address_in_HL()], m_d_reg, cycles, true);
                break;
            case MOV_M_E:
                mov(m_memory[address_in_HL()], m_e_reg, cycles, true);
                break;
            case MOV_M_H:
                mov(m_memory[address_in_HL()], m_h_reg, cycles, true);
                break;
            case MOV_M_L:
                mov(m_memory[address_in_HL()], m_l_reg, cycles, true);
                break;
            case HLT:
                hlt(m_is_stopped, cycles);
                break;
            case MOV_M_A:
                mov(m_memory[address_in_HL()], m_acc_reg, cycles, true);
                break;
            case MOV_A_B:
                mov(m_acc_reg, m_b_reg, cycles);
                break;
            case MOV_A_C:
                mov(m_acc_reg, m_c_reg, cycles);
                break;
            case MOV_A_D:
                mov(m_acc_reg, m_d_reg, cycles);
                break;
            case MOV_A_E:
                mov(m_acc_reg, m_e_reg, cycles);
                break;
            case MOV_A_H:
                mov(m_acc_reg, m_h_reg, cycles);
                break;
            case MOV_A_L:
                mov(m_acc_reg, m_l_reg, cycles);
                break;
            case MOV_A_M:
                mov(m_acc_reg, m_memory[address_in_HL()], cycles, true);
                break;
            case MOV_A_A:
                mov(m_acc_reg, m_acc_reg, cycles);
                break;
            case ADD_B:
                add(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case ADD_C:
                add(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case ADD_D:
                add(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case ADD_E:
                add(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case ADD_H:
                add(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case ADD_L:
                add(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case ADD_M:
                add(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case ADD_A:
                add(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case ADC_B:
                adc(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case ADC_C:
                adc(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case ADC_D:
                adc(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case ADC_E:
                adc(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case ADC_H:
                adc(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case ADC_L:
                adc(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case ADC_M:
                adc(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case ADC_A:
                adc(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case SUB_B:
                sub(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case SUB_C:
                sub(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case SUB_D:
                sub(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case SUB_E:
                sub(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case SUB_H:
                sub(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case SUB_L:
                sub(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case SUB_M:
                sub(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case SUB_A:
                sub(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case SBB_B:
                sbb(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case SBB_C:
                sbb(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case SBB_D:
                sbb(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case SBB_E:
                sbb(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case SBB_H:
                sbb(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case SBB_L:
                sbb(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case SBB_M:
                sbb(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case SBB_A:
                sbb(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case ANA_B:
                ana(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case ANA_C:
                ana(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case ANA_D:
                ana(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case ANA_E:
                ana(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case ANA_H:
                ana(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case ANA_L:
                ana(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case ANA_M:
                ana(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case ANA_A:
                ana(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case XRA_B:
                xra(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case XRA_C:
                xra(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case XRA_D:
                xra(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case XRA_E:
                xra(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case XRA_H:
                xra(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case XRA_L:
                xra(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case XRA_M:
                xra(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case XRA_A:
                xra(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case ORA_B:
                ora(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case ORA_C:
                ora(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case ORA_D:
                ora(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case ORA_E:
                ora(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case ORA_H:
                ora(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case ORA_L:
                ora(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case ORA_M:
                ora(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case ORA_A:
                ora(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
                break;
            case CMP_B:
                cmp(m_acc_reg, m_b_reg, m_flag_reg, cycles);
                break;
            case CMP_C:
                cmp(m_acc_reg, m_c_reg, m_flag_reg, cycles);
                break;
            case CMP_D:
                cmp(m_acc_reg, m_d_reg, m_flag_reg, cycles);
                break;
            case CMP_E:
                cmp(m_acc_reg, m_e_reg, m_flag_reg, cycles);
                break;
            case CMP_H:
                cmp(m_acc_reg, m_h_reg, m_flag_reg, cycles);
                break;
            case CMP_L:
                cmp(m_acc_reg, m_l_reg, m_flag_reg, cycles);
                break;
            case CMP_M:
                cmp(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles, true);
                break;
            case CMP_A:
                cmp(m_acc_reg, m_acc_reg, m_flag_reg, cycles);
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
                xthl(m_h_reg, m_l_reg, m_memory[m_sp], m_memory[m_sp + 1], cycles);
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
            case POS_PSW:
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

    NextByte Cpu::get_next_byte() {
        return {
                .farg = m_memory[m_pc++]
        };
    }

    NextWord Cpu::get_next_word() {
        return {
                .farg = m_memory[m_pc++],
                .sarg = m_memory[m_pc++]
        };
    }

    u16 Cpu::address_in_HL() const {
        return to_u16(m_h_reg, m_l_reg);
    }

    EmulatorMemory &Cpu::memory() {
        return m_memory;
    }

    u16 Cpu::pc() const {
        return m_pc;
    }

    u16 Cpu::sp() const {
        return m_sp;
    }

    u8 Cpu::a() const {
        return m_acc_reg;
    }

    u8 Cpu::b() const {
        return m_b_reg;
    }

    u8 Cpu::c() const {
        return m_c_reg;
    }

    u8 Cpu::d() const {
        return m_d_reg;
    }

    u8 Cpu::e() const {
        return m_e_reg;
    }

    u8 Cpu::h() const {
        return m_h_reg;
    }

    u8 Cpu::l() const {
        return m_l_reg;
    }

    u8 Cpu::f() const {
        return m_flag_reg.to_u8();
    }

    bool Cpu::is_interrupted() const {
        return m_is_interrupted;
    }

    void Cpu::notify_out_observers(u8 port) {
        for (OutObserver *observer: m_out_observers) {
            observer->out_changed(port);
        }
    }

    void Cpu::notify_in_observers(u8 port) {
        for (InObserver *observer: m_in_observers) {
            observer->in_requested(port);
        }
    }

    void Cpu::print_debug() {
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
