#include <algorithm>
#include <iostream>
#include "cpu.h"
#include "8080/next_byte.h"
#include "8080/next_word.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/unrecognized_opcode_exception.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {

    Cpu::Cpu(emu::cpu8080::EmulatorMemory &memory, const std::uint16_t initial_pc) :
            stopped(true),
            inte(false),
            is_interrupted(false),
            instruction_from_interruptor(0),
            memory(memory),
            memory_size(memory.size()),
            opcode(0),
            sp(0),
            pc(initial_pc),
            acc_reg(0),
            b_reg(0),
            c_reg(0),
            d_reg(0),
            e_reg(0),
            h_reg(0),
            l_reg(0) {

        io_in.reserve(number_of_io_ports);
        io_out.reserve(number_of_io_ports);
        for (unsigned int i = 0; i < number_of_io_ports; ++i) {
            io_in.push_back(0);
            io_out.push_back(0);
        }
    }

    Cpu::~Cpu() {
        for (auto observer : out_observers) {
            remove_out_observer(observer);
        }

        for (auto observer : in_observers) {
            remove_in_observer(observer);
        }
    }

    void Cpu::add_out_observer(OutObserver &observer) {
        out_observers.push_back(&observer);
    }

    void Cpu::remove_out_observer(OutObserver *observer) {
        out_observers.erase(
                std::remove(out_observers.begin(), out_observers.end(), observer),
                out_observers.end()
        );
    }

    void Cpu::add_in_observer(InObserver &observer) {
        in_observers.push_back(&observer);
    }

    void Cpu::remove_in_observer(InObserver *observer) {
        in_observers.erase(
                std::remove(in_observers.begin(), in_observers.end(), observer),
                in_observers.end()
        );
    }

    bool Cpu::can_run_next_instruction() const {
        return pc < memory_size && !stopped;
    }

    void Cpu::reset_state() {
        b_reg = 0;
        c_reg = 0;
        d_reg = 0;
        e_reg = 0;
        h_reg = 0;
        l_reg = 0;
        acc_reg = 0;
        flag_reg.reset();
        pc = 0;
        sp = 0;
        stopped = true;
        inte = false;
        is_interrupted = false;
        instruction_from_interruptor = 0;
    }

    void Cpu::start() {
        stopped = false;
    }

    void Cpu::stop() {
        stopped = true;
    }

    void Cpu::interrupt(std::uint8_t instruction_to_perform) {
        is_interrupted = true;
        instruction_from_interruptor = instruction_to_perform;
    }

    bool Cpu::is_inta() const {
        return inte;
    }

    void Cpu::input(int port, std::uint8_t value) {
        io_in[port] = value;
    }

    unsigned long Cpu::next_instruction() {
        unsigned long cycles = 0;

        if (inte && is_interrupted) {
            inte = false;
            is_interrupted = false;
            opcode = instruction_from_interruptor;
        } else {
            opcode = get_next_byte().farg;
        }

        switch (opcode) {
            case NOP:
                nop(cycles);
                break;
            case LXI_B:
                lxi(b_reg, c_reg, get_next_word(), cycles);
                break;
            case STAX_B:
                stax(acc_reg, b_reg, c_reg, memory, cycles);
                break;
            case INX_B:
                inx(b_reg, c_reg, cycles);
                break;
            case INR_B:
                inr(b_reg, flag_reg, cycles);
                break;
            case DCR_B:
                dcr(b_reg, flag_reg, cycles);
                break;
            case MVI_B:
                mvi(b_reg, get_next_byte(), cycles);
                break;
            case RLC_B:
                rlc(acc_reg, flag_reg, cycles);
                break;
            case UNUSED_NOP_1:
                unused_1(opcode, cycles);
                break;
            case DAD_B:
                dad(h_reg, l_reg, emu::util::byte::to_uint16_t(b_reg, c_reg), flag_reg, cycles);
                break;
            case LDAX_B:
                ldax(acc_reg, b_reg, c_reg, memory, cycles);
                break;
            case DCX_B:
                dcx(b_reg, c_reg, cycles);
                break;
            case INR_C:
                inr(c_reg, flag_reg, cycles);
                break;
            case DCR_C:
                dcr(c_reg, flag_reg, cycles);
                break;
            case MVI_C:
                mvi(c_reg, get_next_byte(), cycles);
                break;
            case RRC:
                rrc(acc_reg, flag_reg, cycles);
                break;
            case UNUSED_NOP_2:
                unused_1(opcode, cycles);
                break;
            case LXI_D:
                lxi(d_reg, e_reg, get_next_word(), cycles);
                break;
            case STAX_D:
                stax(acc_reg, d_reg, e_reg, memory, cycles);
                break;
            case INX_D:
                inx(d_reg, e_reg, cycles);
                break;
            case INR_D:
                inr(d_reg, flag_reg, cycles);
                break;
            case DCR_D:
                dcr(d_reg, flag_reg, cycles);
                break;
            case MVI_D:
                mvi(d_reg, get_next_byte(), cycles);
                break;
            case RAL:
                ral(acc_reg, flag_reg, cycles);
                break;
            case UNUSED_NOP_3:
                unused_1(opcode, cycles);
                break;
            case DAD_D:
                dad(h_reg, l_reg, emu::util::byte::to_uint16_t(d_reg, e_reg), flag_reg, cycles);
                break;
            case LDAX_D:
                ldax(acc_reg, d_reg, e_reg, memory, cycles);
                break;
            case DCX_D:
                dcx(d_reg, e_reg, cycles);
                break;
            case INR_E:
                inr(e_reg, flag_reg, cycles);
                break;
            case DCR_E:
                dcr(e_reg, flag_reg, cycles);
                break;
            case MVI_E:
                mvi(e_reg, get_next_byte(), cycles);
                break;
            case RAR:
                rar(acc_reg, flag_reg, cycles);
                break;
            case UNUSED_NOP_4:
                unused_1(opcode, cycles);
                break;
            case LXI_H:
                lxi(h_reg, l_reg, get_next_word(), cycles);
                break;
            case SHLD:
                shld(l_reg, h_reg, memory, get_next_word(), cycles);
                break;
            case INX_H:
                inx(h_reg, l_reg, cycles);
                break;
            case INR_H:
                inr(h_reg, flag_reg, cycles);
                break;
            case DCR_H:
                dcr(h_reg, flag_reg, cycles);
                break;
            case MVI_H:
                mvi(h_reg, get_next_byte(), cycles);
                break;
            case DAA:
                daa(acc_reg, flag_reg, cycles);
                break;
            case UNUSED_NOP_5:
                unused_1(opcode, cycles);
                break;
            case DAD_H:
                dad(h_reg, l_reg, emu::util::byte::to_uint16_t(h_reg, l_reg), flag_reg, cycles);
                break;
            case LHLD:
                lhld(l_reg, h_reg, memory, get_next_word(), cycles);
                break;
            case DCX_H:
                dcx(h_reg, l_reg, cycles);
                break;
            case INR_L:
                inr(l_reg, flag_reg, cycles);
                break;
            case DCR_L:
                dcr(l_reg, flag_reg, cycles);
                break;
            case MVI_L:
                mvi(l_reg, get_next_byte(), cycles);
                break;
            case CMA:
                cma(acc_reg, cycles);
                break;
            case UNUSED_NOP_6:
                unused_1(opcode, cycles);
                break;
            case LXI_SP:
                lxi_sp(sp, get_next_word(), cycles);
                break;
            case STA:
                sta(acc_reg, memory, get_next_word(), cycles);
                break;
            case INX_SP:
                inx_sp(sp, cycles);
                break;
            case INR_M:
                inr(memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case DCR_M:
                dcr(memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case MVI_M:
                mvi(memory[address_in_HL()], get_next_byte(), cycles, true);
                break;
            case STC:
                stc(flag_reg, cycles);
                break;
            case UNUSED_NOP_7:
                unused_1(opcode, cycles);
                break;
            case DAD_SP:
                dad(h_reg, l_reg, sp, flag_reg, cycles);
                break;
            case LDA:
                lda(acc_reg, memory, get_next_word(), cycles);
                break;
            case DCX_SP:
                dcx_sp(sp, cycles);
                break;
            case INR_A:
                inr(acc_reg, flag_reg, cycles);
                break;
            case DCR_A:
                dcr(acc_reg, flag_reg, cycles);
                break;
            case MVI_A:
                mvi(acc_reg, get_next_byte(), cycles);
                break;
            case CMC:
                cmc(flag_reg, cycles);
                break;
            case MOV_B_B:
                mov(b_reg, b_reg, cycles);
                break;
            case MOV_B_C:
                mov(b_reg, c_reg, cycles);
                break;
            case MOV_B_D:
                mov(b_reg, d_reg, cycles);
                break;
            case MOV_B_E:
                mov(b_reg, e_reg, cycles);
                break;
            case MOV_B_H:
                mov(b_reg, h_reg, cycles);
                break;
            case MOV_B_L:
                mov(b_reg, l_reg, cycles);
                break;
            case MOV_B_M:
                mov(b_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_B_A:
                mov(b_reg, acc_reg, cycles);
                break;
            case MOV_C_B:
                mov(c_reg, b_reg, cycles);
                break;
            case MOV_C_C:
                mov(c_reg, c_reg, cycles);
                break;
            case MOV_C_D:
                mov(c_reg, d_reg, cycles);
                break;
            case MOV_C_E:
                mov(c_reg, e_reg, cycles);
                break;
            case MOV_C_H:
                mov(c_reg, h_reg, cycles);
                break;
            case MOV_C_L:
                mov(c_reg, l_reg, cycles);
                break;
            case MOV_C_M:
                mov(c_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_C_A:
                mov(c_reg, acc_reg, cycles);
                break;
            case MOV_D_B:
                mov(d_reg, b_reg, cycles);
                break;
            case MOV_D_C:
                mov(d_reg, c_reg, cycles);
                break;
            case MOV_D_D:
                mov(d_reg, d_reg, cycles);
                break;
            case MOV_D_E:
                mov(d_reg, e_reg, cycles);
                break;
            case MOV_D_H:
                mov(d_reg, h_reg, cycles);
                break;
            case MOV_D_L:
                mov(d_reg, l_reg, cycles);
                break;
            case MOV_D_M:
                mov(d_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_D_A:
                mov(d_reg, acc_reg, cycles);
                break;
            case MOV_E_B:
                mov(e_reg, b_reg, cycles);
                break;
            case MOV_E_C:
                mov(e_reg, c_reg, cycles);
                break;
            case MOV_E_D:
                mov(e_reg, d_reg, cycles);
                break;
            case MOV_E_E:
                mov(e_reg, e_reg, cycles);
                break;
            case MOV_E_H:
                mov(e_reg, h_reg, cycles);
                break;
            case MOV_E_L:
                mov(e_reg, l_reg, cycles);
                break;
            case MOV_E_M:
                mov(e_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_E_A:
                mov(e_reg, acc_reg, cycles);
                break;
            case MOV_H_B:
                mov(h_reg, b_reg, cycles);
                break;
            case MOV_H_C:
                mov(h_reg, c_reg, cycles);
                break;
            case MOV_H_D:
                mov(h_reg, d_reg, cycles);
                break;
            case MOV_H_E:
                mov(h_reg, e_reg, cycles);
                break;
            case MOV_H_H:
                mov(h_reg, h_reg, cycles);
                break;
            case MOV_H_L:
                mov(h_reg, l_reg, cycles);
                break;
            case MOV_H_M:
                mov(h_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_H_A:
                mov(h_reg, acc_reg, cycles);
                break;
            case MOV_L_B:
                mov(l_reg, b_reg, cycles);
                break;
            case MOV_L_C:
                mov(l_reg, c_reg, cycles);
                break;
            case MOV_L_D:
                mov(l_reg, d_reg, cycles);
                break;
            case MOV_L_E:
                mov(l_reg, e_reg, cycles);
                break;
            case MOV_L_H:
                mov(l_reg, h_reg, cycles);
                break;
            case MOV_L_L:
                mov(l_reg, l_reg, cycles);
                break;
            case MOV_L_M:
                mov(l_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_L_A:
                mov(l_reg, acc_reg, cycles);
                break;
            case MOV_M_B:
                mov(memory[address_in_HL()], b_reg, cycles, true);
                break;
            case MOV_M_C:
                mov(memory[address_in_HL()], c_reg, cycles, true);
                break;
            case MOV_M_D:
                mov(memory[address_in_HL()], d_reg, cycles, true);
                break;
            case MOV_M_E:
                mov(memory[address_in_HL()], e_reg, cycles, true);
                break;
            case MOV_M_H:
                mov(memory[address_in_HL()], h_reg, cycles, true);
                break;
            case MOV_M_L:
                mov(memory[address_in_HL()], l_reg, cycles, true);
                break;
            case HLT:
                hlt(stopped, cycles);
                break;
            case MOV_M_A:
                mov(memory[address_in_HL()], acc_reg, cycles, true);
                break;
            case MOV_A_B:
                mov(acc_reg, b_reg, cycles);
                break;
            case MOV_A_C:
                mov(acc_reg, c_reg, cycles);
                break;
            case MOV_A_D:
                mov(acc_reg, d_reg, cycles);
                break;
            case MOV_A_E:
                mov(acc_reg, e_reg, cycles);
                break;
            case MOV_A_H:
                mov(acc_reg, h_reg, cycles);
                break;
            case MOV_A_L:
                mov(acc_reg, l_reg, cycles);
                break;
            case MOV_A_M:
                mov(acc_reg, memory[address_in_HL()], cycles, true);
                break;
            case MOV_A_A:
                mov(acc_reg, acc_reg, cycles);
                break;
            case ADD_B:
                add(acc_reg, b_reg, flag_reg, cycles);
                break;
            case ADD_C:
                add(acc_reg, c_reg, flag_reg, cycles);
                break;
            case ADD_D:
                add(acc_reg, d_reg, flag_reg, cycles);
                break;
            case ADD_E:
                add(acc_reg, e_reg, flag_reg, cycles);
                break;
            case ADD_H:
                add(acc_reg, h_reg, flag_reg, cycles);
                break;
            case ADD_L:
                add(acc_reg, l_reg, flag_reg, cycles);
                break;
            case ADD_M:
                add(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case ADD_A:
                add(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case ADC_B:
                adc(acc_reg, b_reg, flag_reg, cycles);
                break;
            case ADC_C:
                adc(acc_reg, c_reg, flag_reg, cycles);
                break;
            case ADC_D:
                adc(acc_reg, d_reg, flag_reg, cycles);
                break;
            case ADC_E:
                adc(acc_reg, e_reg, flag_reg, cycles);
                break;
            case ADC_H:
                adc(acc_reg, h_reg, flag_reg, cycles);
                break;
            case ADC_L:
                adc(acc_reg, l_reg, flag_reg, cycles);
                break;
            case ADC_M:
                adc(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case ADC_A:
                adc(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case SUB_B:
                sub(acc_reg, b_reg, flag_reg, cycles);
                break;
            case SUB_C:
                sub(acc_reg, c_reg, flag_reg, cycles);
                break;
            case SUB_D:
                sub(acc_reg, d_reg, flag_reg, cycles);
                break;
            case SUB_E:
                sub(acc_reg, e_reg, flag_reg, cycles);
                break;
            case SUB_H:
                sub(acc_reg, h_reg, flag_reg, cycles);
                break;
            case SUB_L:
                sub(acc_reg, l_reg, flag_reg, cycles);
                break;
            case SUB_M:
                sub(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case SUB_A:
                sub(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case SBB_B:
                sbb(acc_reg, b_reg, flag_reg, cycles);
                break;
            case SBB_C:
                sbb(acc_reg, c_reg, flag_reg, cycles);
                break;
            case SBB_D:
                sbb(acc_reg, d_reg, flag_reg, cycles);
                break;
            case SBB_E:
                sbb(acc_reg, e_reg, flag_reg, cycles);
                break;
            case SBB_H:
                sbb(acc_reg, h_reg, flag_reg, cycles);
                break;
            case SBB_L:
                sbb(acc_reg, l_reg, flag_reg, cycles);
                break;
            case SBB_M:
                sbb(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case SBB_A:
                sbb(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case ANA_B:
                ana(acc_reg, b_reg, flag_reg, cycles);
                break;
            case ANA_C:
                ana(acc_reg, c_reg, flag_reg, cycles);
                break;
            case ANA_D:
                ana(acc_reg, d_reg, flag_reg, cycles);
                break;
            case ANA_E:
                ana(acc_reg, e_reg, flag_reg, cycles);
                break;
            case ANA_H:
                ana(acc_reg, h_reg, flag_reg, cycles);
                break;
            case ANA_L:
                ana(acc_reg, l_reg, flag_reg, cycles);
                break;
            case ANA_M:
                ana(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case ANA_A:
                ana(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case XRA_B:
                xra(acc_reg, b_reg, flag_reg, cycles);
                break;
            case XRA_C:
                xra(acc_reg, c_reg, flag_reg, cycles);
                break;
            case XRA_D:
                xra(acc_reg, d_reg, flag_reg, cycles);
                break;
            case XRA_E:
                xra(acc_reg, e_reg, flag_reg, cycles);
                break;
            case XRA_H:
                xra(acc_reg, h_reg, flag_reg, cycles);
                break;
            case XRA_L:
                xra(acc_reg, l_reg, flag_reg, cycles);
                break;
            case XRA_M:
                xra(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case XRA_A:
                xra(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case ORA_B:
                ora(acc_reg, b_reg, flag_reg, cycles);
                break;
            case ORA_C:
                ora(acc_reg, c_reg, flag_reg, cycles);
                break;
            case ORA_D:
                ora(acc_reg, d_reg, flag_reg, cycles);
                break;
            case ORA_E:
                ora(acc_reg, e_reg, flag_reg, cycles);
                break;
            case ORA_H:
                ora(acc_reg, h_reg, flag_reg, cycles);
                break;
            case ORA_L:
                ora(acc_reg, l_reg, flag_reg, cycles);
                break;
            case ORA_M:
                ora(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case ORA_A:
                ora(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case CMP_B:
                cmp(acc_reg, b_reg, flag_reg, cycles);
                break;
            case CMP_C:
                cmp(acc_reg, c_reg, flag_reg, cycles);
                break;
            case CMP_D:
                cmp(acc_reg, d_reg, flag_reg, cycles);
                break;
            case CMP_E:
                cmp(acc_reg, e_reg, flag_reg, cycles);
                break;
            case CMP_H:
                cmp(acc_reg, h_reg, flag_reg, cycles);
                break;
            case CMP_L:
                cmp(acc_reg, l_reg, flag_reg, cycles);
                break;
            case CMP_M:
                cmp(acc_reg, memory[address_in_HL()], flag_reg, cycles, true);
                break;
            case CMP_A:
                cmp(acc_reg, acc_reg, flag_reg, cycles);
                break;
            case RNZ:
                rnz(pc, sp, memory, flag_reg, cycles);
                break;
            case POP_B:
                pop(b_reg, c_reg, sp, memory, cycles);
                break;
            case JNZ:
                jnz(pc, get_next_word(), flag_reg, cycles);
                break;
            case JMP:
                jmp(pc, get_next_word(), cycles);
                break;
            case CNZ:
                cnz(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case PUSH_B:
                push(b_reg, c_reg, sp, memory, cycles);
                break;
            case ADI:
                adi(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_0:
                rst_0(pc, sp, memory, cycles);
                break;
            case RZ:
                rz(pc, sp, memory, flag_reg, cycles);
                break;
            case RET:
                ret(pc, sp, memory, cycles);
                break;
            case JZ:
                jz(pc, get_next_word(), flag_reg, cycles);
                break;
            case UNUSED_JMP_1:
                unused_3(opcode, pc, cycles);
                break;
            case CZ:
                cz(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case CALL:
                call(pc, sp, memory, get_next_word(), cycles);
                break;
            case ACI:
                aci(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_1:
                rst_1(pc, sp, memory, cycles);
                break;
            case RNC:
                rnc(pc, sp, memory, flag_reg, cycles);
                break;
            case POP_D:
                pop(d_reg, e_reg, sp, memory, cycles);
                break;
            case JNC:
                jnc(pc, get_next_word(), flag_reg, cycles);
                break;
            case OUT: {
                NextByte args = get_next_byte();
                out(acc_reg, args, io_out, cycles);
                notify_out_observers(args.farg);
                break;
            }
            case CNC:
                cnc(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case PUSH_D:
                push(d_reg, e_reg, sp, memory, cycles);
                break;
            case SUI:
                sui(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_2:
                rst_2(pc, sp, memory, cycles);
                break;
            case RC:
                rc(pc, sp, memory, flag_reg, cycles);
                break;
            case UNUSED_RET_1:
                unused_1(opcode, cycles);
                break;
            case JC:
                jc(pc, get_next_word(), flag_reg, cycles);
                break;
            case IN: {
                NextByte args = get_next_byte();
                notify_in_observers(args.farg);
                in(acc_reg, args, io_in, cycles);
                break;
            }
            case CC:
                cc(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case UNUSED_CALL_1:
                unused_3(opcode, pc, cycles);
                break;
            case SBI:
                sbi(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_3:
                rst_3(pc, sp, memory, cycles);
                break;
            case RPO:
                rpo(pc, sp, memory, flag_reg, cycles);
                break;
            case POP_H:
                pop(h_reg, l_reg, sp, memory, cycles);
                break;
            case JPO:
                jpo(pc, get_next_word(), flag_reg, cycles);
                break;
            case XTHL:
                xthl(h_reg, l_reg, memory[sp], memory[sp + 1], cycles);
                break;
            case CPO:
                cpo(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case PUSH_H:
                push(h_reg, l_reg, sp, memory, cycles);
                break;
            case ANI:
                ani(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_4:
                rst_4(pc, sp, memory, cycles);
                break;
            case RPE:
                rpe(pc, sp, memory, flag_reg, cycles);
                break;
            case PCHL:
                pchl(pc, address_in_HL(), cycles);
                break;
            case JPE:
                jpe(pc, get_next_word(), flag_reg, cycles);
                break;
            case XCHG:
                xchg(h_reg, l_reg, d_reg, e_reg, cycles);
                break;
            case CPE:
                cpe(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case UNUSED_CALL_2:
                unused_3(opcode, pc, cycles);
                break;
            case XRI:
                xri(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_5:
                rst_5(pc, sp, memory, cycles);
                break;
            case RP:
                rp(pc, sp, memory, flag_reg, cycles);
                break;
            case POS_PSW:
                pop_psw(flag_reg, acc_reg, sp, memory, cycles);
                break;
            case JP:
                jp(pc, get_next_word(), flag_reg, cycles);
                break;
            case DI:
                di(inte, cycles);
                break;
            case CP:
                cp(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case PUSH_PSW:
                push_psw(flag_reg, acc_reg, sp, memory, cycles);
                break;
            case ORI:
                ori(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_6:
                rst_6(pc, sp, memory, cycles);
                break;
            case RM:
                rm(pc, sp, memory, flag_reg, cycles);
                break;
            case SPHL:
                sphl(sp, address_in_HL(), cycles);
                break;
            case JM:
                jm(pc, get_next_word(), flag_reg, cycles);
                break;
            case EI:
                ei(inte, cycles);
                break;
            case CM:
                cm(pc, sp, memory, get_next_word(), flag_reg, cycles);
                break;
            case UNUSED_CALL_3:
                unused_3(opcode, pc, cycles);
                break;
            case CPI:
                cpi(acc_reg, get_next_byte(), flag_reg, cycles);
                break;
            case RST_7:
                rst_7(pc, sp, memory, cycles);
                break;
            default:
                throw emu::util::exceptions::UnrecognizedOpcodeException(opcode);
        }

        return cycles;
    }

    NextByte Cpu::get_next_byte() {
        return {
                .farg = memory[pc++]
        };
    }

    NextWord Cpu::get_next_word() {
        return {
                .farg = memory[pc++],
                .sarg = memory[pc++]
        };
    }

    std::uint16_t Cpu::address_in_HL() const {
        return emu::util::byte::to_uint16_t(h_reg, l_reg);
    }

    emu::cpu8080::EmulatorMemory &Cpu::Memory() {
        return memory;
    }

    std::uint16_t Cpu::PC() const {
        return pc;
    }

    std::uint8_t Cpu::A() const {
        return acc_reg;
    }

    std::uint8_t Cpu::B() const {
        return b_reg;
    }

    std::uint8_t Cpu::C() const {
        return c_reg;
    }

    std::uint8_t Cpu::D() const {
        return d_reg;
    }

    std::uint8_t Cpu::E() const {
        return e_reg;
    }

    std::uint8_t Cpu::H() const {
        return h_reg;
    }

    std::uint8_t Cpu::L() const {
        return l_reg;
    }

    void Cpu::notify_out_observers(std::uint8_t port) {
        for (OutObserver *observer: out_observers) {
            observer->out_changed(port);
        }
    }

    void Cpu::notify_in_observers(std::uint8_t port) {
        for (InObserver *observer: in_observers) {
            observer->in_requested(port);
        }
    }

    void Cpu::print_debug() {
        std::cout << "PC=" << emu::util::string::hexify(pc)
                  << ",SP=" << emu::util::string::hexify(sp)
                  << ",op=" << emu::util::string::hexify(opcode)
                  << ",a=" << emu::util::string::hexify(acc_reg)
                  << ",b=" << emu::util::string::hexify(b_reg)
                  << ",c=" << emu::util::string::hexify(c_reg)
                  << ",d=" << emu::util::string::hexify(d_reg)
                  << ",e=" << emu::util::string::hexify(e_reg)
                  << ",h=" << emu::util::string::hexify(h_reg)
                  << ",l=" << emu::util::string::hexify(l_reg)
                  << ",ca=" << flag_reg.is_carry_flag_set()
                  << ",pa=" << flag_reg.is_parity_flag_set()
                  << ",ac=" << flag_reg.is_aux_carry_flag_set()
                  << ",z=" << flag_reg.is_zero_flag_set()
                  << ",s=" << flag_reg.is_sign_flag_set()
                  << "\n";
    }
}
