#include <algorithm>
#include <iostream>
#include "cpu.h"
#include "z80/instructions/instructions.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify;
    using emu::exceptions::UnrecognizedOpcodeException;

    Cpu::Cpu(
            EmulatorMemory &memory,
            const u16 initial_pc
    ) :
            m_is_stopped(true),
            m_iff1(false),
            m_iff2(false),
            m_is_interrupted(false),
            m_instruction_from_interruptor(0),
            m_memory(memory),
            m_memory_size(memory.size()),
            m_opcode(0),
            m_sp(0xffff),
            m_pc(initial_pc),
            m_acc_reg(0xff),
            m_acc_p_reg(0),
            m_b_reg(0),
            m_b_p_reg(0),
            m_c_reg(0),
            m_c_p_reg(0),
            m_d_reg(0),
            m_d_p_reg(0),
            m_e_reg(0),
            m_e_p_reg(0),
            m_h_reg(0),
            m_h_p_reg(0),
            m_l_reg(0),
            m_l_p_reg(0),
            m_ix_reg(0),
            m_iy_reg(0),
            m_i_reg(0),
            m_r_reg(0) {
        m_io_in.reserve(number_of_io_ports);
        m_io_out.reserve(number_of_io_ports);
        for (unsigned int i = 0; i < number_of_io_ports; ++i) {
            m_io_in.push_back(0);
            m_io_out.push_back(0);
        }
        m_flag_reg.from_u8(0xff);
        m_flag_p_reg.from_u8(0x00);
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
        m_acc_reg = 0xff;
        m_acc_p_reg = 0;
        m_b_reg = 0;
        m_b_p_reg = 0;
        m_c_reg = 0;
        m_c_p_reg = 0;
        m_d_reg = 0;
        m_d_p_reg = 0;
        m_e_reg = 0;
        m_e_p_reg = 0;
        m_h_reg = 0;
        m_h_p_reg = 0;
        m_l_reg = 0;
        m_l_p_reg = 0;
        m_ix_reg = 0;
        m_iy_reg = 0;
        m_i_reg = 0;
        m_r_reg = 0;
        m_flag_reg.from_u8(0xff);
        m_flag_p_reg.from_u8(0x00);
        m_pc = 0;
        m_sp = 0xffff;
        m_is_stopped = true;
        m_iff1 = false;
        m_iff2 = false;
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
        return m_iff1;
    }

    void Cpu::input(int port, u8 value) {
        m_io_in[port] = value;
    }

    unsigned long Cpu::next_instruction() {
        unsigned long cycles = 0;

        if (m_iff1 && m_is_interrupted) {
            m_iff1 = false;
            m_iff2 = false;
            m_is_interrupted = false;
            m_opcode = m_instruction_from_interruptor;
        } else {
            m_opcode = get_next_byte().farg;
        }

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
                ld_Mss_A(m_memory[to_u16(m_b_reg, m_c_reg)], m_acc_reg, cycles);
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
                ld_A_Mss(m_acc_reg, m_memory[to_u16(m_b_reg, m_c_reg)], cycles);
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
                ld_Mss_A(m_memory[to_u16(m_d_reg, m_e_reg)], m_acc_reg, cycles);
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
                ld_A_Mss(m_acc_reg, m_memory[to_u16(m_d_reg, m_e_reg)], cycles);
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
                ld_Mnn_HL(m_l_reg, m_h_reg, m_memory, get_next_word(), cycles);
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
                ld_HL_Mnn(m_l_reg, m_h_reg, m_memory, get_next_word(), cycles);
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
                inc_MHL(m_memory[address_in_HL()], m_flag_reg, cycles);
                break;
            case DEC_MHL:
                dec_MHL(m_memory[address_in_HL()], m_flag_reg, cycles);
                break;
            case LD_MHL_n:
                ld_MHL_n(m_memory[address_in_HL()], get_next_byte(), cycles);
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
                ld_r_MHL(m_b_reg, m_memory[address_in_HL()], cycles);
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
                ld_r_MHL(m_c_reg, m_memory[address_in_HL()], cycles);
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
                ld_r_MHL(m_d_reg, m_memory[address_in_HL()], cycles);
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
                ld_r_MHL(m_e_reg, m_memory[address_in_HL()], cycles);
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
                ld_r_MHL(m_h_reg, m_memory[address_in_HL()], cycles);
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
                ld_r_MHL(m_l_reg, m_memory[address_in_HL()], cycles);
                break;
            case LD_L_A:
                ld_r_r(m_l_reg, m_acc_reg, cycles);
                break;
            case LD_MHL_B:
                ld_MHL_r(m_memory[address_in_HL()], m_b_reg, cycles);
                break;
            case LD_MHL_C:
                ld_MHL_r(m_memory[address_in_HL()], m_c_reg, cycles);
                break;
            case LD_MHL_D:
                ld_MHL_r(m_memory[address_in_HL()], m_d_reg, cycles);
                break;
            case LD_MHL_E:
                ld_MHL_r(m_memory[address_in_HL()], m_e_reg, cycles);
                break;
            case LD_MHL_H:
                ld_MHL_r(m_memory[address_in_HL()], m_h_reg, cycles);
                break;
            case LD_MHL_L:
                ld_MHL_r(m_memory[address_in_HL()], m_l_reg, cycles);
                break;
            case HALT:
                halt(m_is_stopped, cycles);
                break;
            case LD_MHL_A:
                ld_MHL_r(m_memory[address_in_HL()], m_acc_reg, cycles);
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
                ld_r_MHL(m_acc_reg, m_memory[address_in_HL()], cycles);
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
                add_A_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                adc_A_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                sub_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                sbc_A_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                and_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                xor_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                or_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                cp_MHL(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
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
                unused_3(m_opcode, m_pc, cycles);
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
                out_Mn_A(m_acc_reg, args, m_io_out, cycles);
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
                in_A_Mn(m_acc_reg, args, m_io_in, cycles);
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

    void Cpu::next_ixy_instruction(u8 ixy_opcode, u16 &ixy_reg, unsigned long cycles) {
        print_debug(ixy_opcode);
        r_tick();

        switch (ixy_opcode) {
            case ADD_IXY_BC:
                add_ixy_pp(ixy_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
                break;
            case ADD_IXY_DE:
                add_ixy_pp(ixy_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
                break;
            case LD_IXY_nn:
                ld_ixy_nn(ixy_reg, get_next_word(), cycles);
                break;
            case LD_Mnn_IXY:
                ld_Mnn_ixy(get_next_byte(), memory(), ixy_reg, cycles);
                break;
            case INC_IXY:
                inc_ixy(ixy_reg, cycles);
                break;
            case ADD_IXY_IXY:
                add_ixy_pp(ixy_reg, ixy_reg, m_flag_reg, cycles);
                break;
            case LD_IXY_Mnn:
                ld_ixy_Mnn(ixy_reg, get_next_byte(), memory(), cycles);
                break;
            case DEC_IXY:
                dec_ixy(ixy_reg, cycles);
                break;
            case ADD_IX_SP:
                add_ixy_pp(ixy_reg, m_sp, m_flag_reg, cycles);
                break;
            case LD_B_MIXY_P_n:
                ld_r_MixyPd(m_b_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_C_MIXY_P_n:
                ld_r_MixyPd(m_c_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_D_MIXY_P_n:
                ld_r_MixyPd(m_d_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_E_MIXY_P_n:
                ld_r_MixyPd(m_e_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_H_MIXY_P_n:
                ld_r_MixyPd(m_h_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_L_MIXY_P_n:
                ld_r_MixyPd(m_l_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case LD_A_MIXY_P_n:
                ld_r_MixyPd(m_acc_reg, ixy_reg, get_next_byte(), m_memory, cycles);
                break;
            case ADD_A_IXH_UNDOC:
                add_A_ixy_h_or_l(m_acc_reg, second_byte(ixy_reg), m_flag_reg, cycles);
                break;
            case ADD_A_IXL_UNDOC:
                add_A_ixy_h_or_l(m_acc_reg, first_byte(ixy_reg), m_flag_reg, cycles);
                break;
            case ADC_A_IXH_UNDOC:
                adc_A_ixy_h_or_l(m_acc_reg, second_byte(ixy_reg), m_flag_reg, cycles);
                break;
            case ADC_A_IXL_UNDOC:
                adc_A_ixy_h_or_l(m_acc_reg, first_byte(ixy_reg), m_flag_reg, cycles);
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
            case LD_SP_IX:
                ld_sp_ixy(m_sp, m_ix_reg, cycles);
                break;
            default:
                throw UnrecognizedOpcodeException(ixy_opcode, "IX/IY instructions");
        }
    }

    void Cpu::next_extd_instruction(u8 extd_opcode, unsigned long cycles) {
        print_debug(extd_opcode);
        r_tick();

        switch (extd_opcode) {
            case SBC_HL_BC:
                sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
                break;
            case SBC_HL_DE:
                sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
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
            case ADC_HL_BC:
                adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_b_reg, m_c_reg), m_flag_reg, cycles);
                break;
            case ADC_HL_DE:
                adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_d_reg, m_e_reg), m_flag_reg, cycles);
                break;
            case SBC_HL_HL:
                sbc_HL_ss(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
                break;
            case RRD:
                rrd(m_acc_reg, m_memory[address_in_HL()], m_flag_reg, cycles);
                break;
            case ADC_HL_HL:
                adc_hl_ss(m_h_reg, m_l_reg, to_u16(m_h_reg, m_l_reg), m_flag_reg, cycles);
                break;
            case SBC_HL_SP:
                sbc_HL_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
                break;
            case LD_Mnn_sp:
                ld_Mnn_sp(m_sp, m_memory, get_next_word(), cycles);
                break;
            case ADC_HL_SP:
                adc_hl_ss(m_h_reg, m_l_reg, m_sp, m_flag_reg, cycles);
                break;
            case LD_sp_Mnn:
                ld_sp_Mnn(m_sp, m_memory, get_next_word(), cycles);
                break;
            case LDI:
                throw UnrecognizedOpcodeException(extd_opcode, "EXTD instructions");
            case LDIR:
                ldir(m_pc, m_b_reg, m_c_reg, m_d_reg, m_e_reg,
                     m_h_reg, m_l_reg, m_acc_reg, m_memory, m_flag_reg, cycles);
                break;
            default:
                throw UnrecognizedOpcodeException(extd_opcode, "EXTD instructions");
        }
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

    u16 Cpu::address_in_DE() const {
        return to_u16(m_d_reg, m_e_reg);
    }

    u16 Cpu::address_in_HL_p() const {
        return to_u16(m_h_p_reg, m_l_p_reg);
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

    u8 Cpu::a_p() const {
        return m_acc_p_reg;
    }

    u8 Cpu::b() const {
        return m_b_reg;
    }

    u8 Cpu::b_p() const {
        return m_b_p_reg;
    }

    u8 Cpu::c() const {
        return m_c_reg;
    }

    u8 Cpu::c_p() const {
        return m_c_p_reg;
    }

    u8 Cpu::d() const {
        return m_d_reg;
    }

    u8 Cpu::d_p() const {
        return m_d_p_reg;
    }

    u8 Cpu::e() const {
        return m_e_reg;
    }

    u8 Cpu::e_p() const {
        return m_e_p_reg;
    }

    u8 Cpu::h() const {
        return m_h_reg;
    }

    u8 Cpu::h_p() const {
        return m_h_p_reg;
    }

    u8 Cpu::l() const {
        return m_l_reg;
    }

    u8 Cpu::l_p() const {
        return m_l_p_reg;
    }

    u8 Cpu::f() const {
        return m_flag_reg.to_u8();
    }

    u8 Cpu::f_p() const {
        return m_flag_p_reg.to_u8();
    }

    u16 Cpu::ix() const {
        return m_ix_reg;
    }

    u16 Cpu::iy() const {
        return m_iy_reg;
    }

    u8 Cpu::i() const {
        return m_i_reg;
    }

    u8 Cpu::r() const {
        return m_r_reg;
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

    void Cpu::r_tick() {
        m_r_reg = m_r_reg == INT8_MAX ? 0 : m_r_reg + 1;
    }

    void Cpu::print_debug(u8 opcode) {
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
                      << "\n";
        }
    }
}
