#include <iostream>
#include "disassembler8080.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/string_util.h"
#include "crosscutting/unrecognized_opcode_exception.h"

namespace emu::cpu8080 {

    Disassembler8080::Disassembler8080(emu::cpu8080::EmulatorMemory &memory)
            : m_memory(memory),
              m_memory_size(memory.size()),
              m_pc(0),
              m_opcode(0) {
    }

    void Disassembler8080::disassemble() {
        while (m_pc < m_memory_size) {
            print_next_instruction();
        }
    }

    void Disassembler8080::print_next_instruction() {
        std::cout << emu::util::string::hexify_wo_0x(m_pc, 4) << "\t\t";

        m_opcode = get_next_byte().farg;

        switch (m_opcode) {
        case NOP:
            print_nop();
            break;
        case LXI_B:
            print_lxi("B", get_next_word());
            break;
        case STAX_B:
            print_stax("B");
            break;
        case INX_B:
            print_inx("B");
            break;
        case INR_B:
            print_inr("B");
            break;
        case DCR_B:
            print_dcr("B");
            break;
        case MVI_B:
            print_mvi("B", get_next_byte());
            break;
        case RLC_B:
            print_rlc("B");
            break;
        case UNUSED_NOP_1:
            print_unused_nop();
            break;
        case DAD_B:
            print_dad("B");
            break;
        case LDAX_B:
            print_ldax("B");
            break;
        case DCX_B:
            print_dcx("B");
            break;
        case INR_C:
            print_inr("C");
            break;
        case DCR_C:
            print_dcr("C");
            break;
        case MVI_C:
            print_mvi("C", get_next_byte());
            break;
        case RRC:
            print_rrc();
            break;
        case UNUSED_NOP_2:
            print_unused_nop();
            break;
        case LXI_D:
            print_lxi("D", get_next_word());
            break;
        case STAX_D:
            print_stax("D");
            break;
        case INX_D:
            print_inx("D");
            break;
        case INR_D:
            print_inr("D");
            break;
        case DCR_D:
            print_dcr("D");
            break;
        case MVI_D:
            print_mvi("D", get_next_byte());
            break;
        case RAL:
            print_ral();
            break;
        case UNUSED_NOP_3:
            print_unused_nop();
            break;
        case DAD_D:
            print_dad("D");
            break;
        case LDAX_D:
            print_ldax("D");
            break;
        case DCX_D:
            print_dcx("D");
            break;
        case INR_E:
            print_inr("E");
            break;
        case DCR_E:
            print_dcr("E");
            break;
        case MVI_E:
            print_mvi("E", get_next_byte());
            break;
        case RAR:
            print_rar();
            break;
        case UNUSED_NOP_4:
            print_unused_nop();
            break;
        case LXI_H:
            print_lxi("H", get_next_word());
            break;
        case SHLD:
            print_shld(get_next_word());
            break;
        case INX_H:
            print_inx("H");
            break;
        case INR_H:
            print_inr("H");
            break;
        case DCR_H:
            print_dcr("H");
            break;
        case MVI_H:
            print_mvi("H", get_next_byte());
            break;
        case DAA:
            print_daa();
            break;
        case UNUSED_NOP_5:
            print_unused_nop();
            break;
        case DAD_H:
            print_dad("H");
            break;
        case LHLD:
            print_lhld(get_next_word());
            break;
        case DCX_H:
            print_dcx("H");
            break;
        case INR_L:
            print_inr("L");
            break;
        case DCR_L:
            print_dcr("L");
            break;
        case MVI_L:
            print_mvi("L", get_next_byte());
            break;
        case CMA:
            print_cma();
            break;
        case UNUSED_NOP_6:
            print_unused_nop();
            break;
        case LXI_SP:
            print_lxi("SP", get_next_word());
            break;
        case STA:
            print_sta(get_next_word());
            break;
        case INX_SP:
            print_inx("SP");
            break;
        case INR_M:
            print_inr("M");
            break;
        case DCR_M:
            print_dcr("M");
            break;
        case MVI_M:
            print_mvi("M", get_next_byte());
            break;
        case STC:
            print_stc();
            break;
        case UNUSED_NOP_7:
            print_unused_nop();
            break;
        case DAD_SP:
            print_dad("SP");
            break;
        case LDA:
            print_lda(get_next_word());
            break;
        case DCX_SP:
            print_dcx("SP");
            break;
        case INR_A:
            print_inr("A");
            break;
        case DCR_A:
            print_dcr("A");
            break;
        case MVI_A:
            print_mvi("A", get_next_byte());
            break;
        case CMC:
            print_cmc();
            break;
        case MOV_B_B:
            print_mov("B", "B");
            break;
        case MOV_B_C:
            print_mov("B", "C");
            break;
        case MOV_B_D:
            print_mov("B", "D");
            break;
        case MOV_B_E:
            print_mov("B", "E");
            break;
        case MOV_B_H:
            print_mov("B", "H");
            break;
        case MOV_B_L:
            print_mov("B", "L");
            break;
        case MOV_B_M:
            print_mov("B", "M");
            break;
        case MOV_B_A:
            print_mov("B", "A");
            break;
        case MOV_C_B:
            print_mov("C", "B");
            break;
        case MOV_C_C:
            print_mov("C", "C");
            break;
        case MOV_C_D:
            print_mov("C", "D");
            break;
        case MOV_C_E:
            print_mov("C", "E");
            break;
        case MOV_C_H:
            print_mov("C", "H");
            break;
        case MOV_C_L:
            print_mov("C", "L");
            break;
        case MOV_C_M:
            print_mov("C", "M");
            break;
        case MOV_C_A:
            print_mov("C", "A");
            break;
        case MOV_D_B:
            print_mov("D", "B");
            break;
        case MOV_D_C:
            print_mov("D", "C");
            break;
        case MOV_D_D:
            print_mov("D", "D");
            break;
        case MOV_D_E:
            print_mov("D", "E");
            break;
        case MOV_D_H:
            print_mov("D", "H");
            break;
        case MOV_D_L:
            print_mov("D", "L");
            break;
        case MOV_D_M:
            print_mov("D", "M");
            break;
        case MOV_D_A:
            print_mov("D", "A");
            break;
        case MOV_E_B:
            print_mov("E", "B");
            break;
        case MOV_E_C:
            print_mov("E", "C");
            break;
        case MOV_E_D:
            print_mov("E", "D");
            break;
        case MOV_E_E:
            print_mov("E", "E");
            break;
        case MOV_E_H:
            print_mov("E", "H");
            break;
        case MOV_E_L:
            print_mov("E", "L");
            break;
        case MOV_E_M:
            print_mov("E", "M");
            break;
        case MOV_E_A:
            print_mov("E", "A");
            break;
        case MOV_H_B:
            print_mov("H", "B");
            break;
        case MOV_H_C:
            print_mov("H", "C");
            break;
        case MOV_H_D:
            print_mov("H", "D");
            break;
        case MOV_H_E:
            print_mov("H", "E");
            break;
        case MOV_H_H:
            print_mov("H", "H");
            break;
        case MOV_H_L:
            print_mov("H", "L");
            break;
        case MOV_H_M:
            print_mov("H", "M");
            break;
        case MOV_H_A:
            print_mov("H", "A");
            break;
        case MOV_L_B:
            print_mov("L", "B");
            break;
        case MOV_L_C:
            print_mov("L", "C");
            break;
        case MOV_L_D:
            print_mov("L", "D");
            break;
        case MOV_L_E:
            print_mov("L", "E");
            break;
        case MOV_L_H:
            print_mov("L", "H");
            break;
        case MOV_L_L:
            print_mov("L", "L");
            break;
        case MOV_L_M:
            print_mov("L", "M");
            break;
        case MOV_L_A:
            print_mov("L", "A");
            break;
        case MOV_M_B:
            print_mov("M", "B");
            break;
        case MOV_M_C:
            print_mov("M", "C");
            break;
        case MOV_M_D:
            print_mov("M", "D");
            break;
        case MOV_M_E:
            print_mov("M", "E");
            break;
        case MOV_M_H:
            print_mov("M", "H");
            break;
        case MOV_M_L:
            print_mov("M", "L");
            break;
        case HLT:
            print_hlt();
            break;
        case MOV_M_A:
            print_mov("M", "A");
            break;
        case MOV_A_B:
            print_mov("A", "B");
            break;
        case MOV_A_C:
            print_mov("A", "C");
            break;
        case MOV_A_D:
            print_mov("A", "D");
            break;
        case MOV_A_E:
            print_mov("A", "E");
            break;
        case MOV_A_H:
            print_mov("A", "H");
            break;
        case MOV_A_L:
            print_mov("A", "L");
            break;
        case MOV_A_M:
            print_mov("A", "M");
            break;
        case MOV_A_A:
            print_mov("A", "A");
            break;
        case ADD_B:
            print_add("B");
            break;
        case ADD_C:
            print_add("C");
            break;
        case ADD_D:
            print_add("D");
            break;
        case ADD_E:
            print_add("E");
            break;
        case ADD_H:
            print_add("H");
            break;
        case ADD_L:
            print_add("L");
            break;
        case ADD_M:
            print_add("M");
            break;
        case ADD_A:
            print_add("A");
            break;
        case ADC_B:
            print_adc("B");
            break;
        case ADC_C:
            print_adc("C");
            break;
        case ADC_D:
            print_adc("D");
            break;
        case ADC_E:
            print_adc("E");
            break;
        case ADC_H:
            print_adc("H");
            break;
        case ADC_L:
            print_adc("L");
            break;
        case ADC_M:
            print_adc("M");
            break;
        case ADC_A:
            print_adc("A");
            break;
        case SUB_B:
            print_sub("B");
            break;
        case SUB_C:
            print_sub("C");
            break;
        case SUB_D:
            print_sub("D");
            break;
        case SUB_E:
            print_sub("E");
            break;
        case SUB_H:
            print_sub("H");
            break;
        case SUB_L:
            print_sub("L");
            break;
        case SUB_M:
            print_sub("M");
            break;
        case SUB_A:
            print_sub("A");
            break;
        case SBB_B:
            print_sbb("B");
            break;
        case SBB_C:
            print_sbb("C");
            break;
        case SBB_D:
            print_sbb("D");
            break;
        case SBB_E:
            print_sbb("E");
            break;
        case SBB_H:
            print_sbb("H");
            break;
        case SBB_L:
            print_sbb("L");
            break;
        case SBB_M:
            print_sbb("M");
            break;
        case SBB_A:
            print_sbb("A");
            break;
        case ANA_B:
            print_ana("B");
            break;
        case ANA_C:
            print_ana("C");
            break;
        case ANA_D:
            print_ana("D");
            break;
        case ANA_E:
            print_ana("E");
            break;
        case ANA_H:
            print_ana("H");
            break;
        case ANA_L:
            print_ana("L");
            break;
        case ANA_M:
            print_ana("M");
            break;
        case ANA_A:
            print_ana("A");
            break;
        case XRA_B:
            print_xra("B");
            break;
        case XRA_C:
            print_xra("C");
            break;
        case XRA_D:
            print_xra("D");
            break;
        case XRA_E:
            print_xra("E");
            break;
        case XRA_H:
            print_xra("H");
            break;
        case XRA_L:
            print_xra("L");
            break;
        case XRA_M:
            print_xra("M");
            break;
        case XRA_A:
            print_xra("A");
            break;
        case ORA_B:
            print_ora("B");
            break;
        case ORA_C:
            print_ora("C");
            break;
        case ORA_D:
            print_ora("D");
            break;
        case ORA_E:
            print_ora("E");
            break;
        case ORA_H:
            print_ora("H");
            break;
        case ORA_L:
            print_ora("L");
            break;
        case ORA_M:
            print_ora("M");
            break;
        case ORA_A:
            print_ora("A");
            break;
        case CMP_B:
            print_cmp("B");
            break;
        case CMP_C:
            print_cmp("C");
            break;
        case CMP_D:
            print_cmp("D");
            break;
        case CMP_E:
            print_cmp("E");
            break;
        case CMP_H:
            print_cmp("H");
            break;
        case CMP_L:
            print_cmp("L");
            break;
        case CMP_M:
            print_cmp("M");
            break;
        case CMP_A:
            print_cmp("A");
            break;
        case RNZ:
            print_rnz();
            break;
        case POP_B:
            print_pop("B");
            break;
        case JNZ:
            print_jnz(get_next_word());
            break;
        case JMP:
            print_jmp(get_next_word());
            break;
        case CNZ:
            print_cnz(get_next_word());
            break;
        case PUSH_B:
            print_push("B");
            break;
        case ADI:
            print_adi(get_next_byte());
            break;
        case RST_0:
            print_rst(0);
            break;
        case RZ:
            print_rz();
            break;
        case RET:
            print_ret();
            break;
        case JZ:
            print_jz(get_next_word());
            break;
        case UNUSED_JMP_1:
            print_unused_jmp(get_next_word());
            break;
        case CZ:
            print_cz(get_next_word());
            break;
        case CALL:
            print_call(get_next_word());
            break;
        case ACI:
            print_aci(get_next_byte());
            break;
        case RST_1:
            print_rst(1);
            break;
        case RNC:
            print_rnc();
            break;
        case POP_D:
            print_pop("D");
            break;
        case JNC:
            print_jnc(get_next_word());
            break;
        case OUT:
            print_out(get_next_byte());
            break;
        case CNC:
            print_cnc(get_next_word());
            break;
        case PUSH_D:
            print_push("D");
            break;
        case SUI:
            print_sui(get_next_byte());
            break;
        case RST_2:
            print_rst(2);
            break;
        case RC:
            print_rc();
            break;
        case UNUSED_RET_1:
            print_unused_ret();
            break;
        case JC:
            print_jc(get_next_word());
            break;
        case IN:
            print_in(get_next_byte());
            break;
        case CC:
            print_cc(get_next_word());
            break;
        case UNUSED_CALL_1:
            print_unused_call(get_next_word());
            break;
        case SBI:
            print_sbi(get_next_byte());
            break;
        case RST_3:
            print_rst(3);
            break;
        case RPO:
            print_rpo();
            break;
        case POP_H:
            print_pop("H");
            break;
        case JPO:
            print_jpo(get_next_word());
            break;
        case XTHL:
            print_xthl();
            break;
        case CPO:
            print_cpo(get_next_word());
            break;
        case PUSH_H:
            print_push("H");
            break;
        case ANI:
            print_ani(get_next_byte());
            break;
        case RST_4:
            print_rst(4);
            break;
        case RPE:
            print_rpe();
            break;
        case PCHL:
            print_pchl();
            break;
        case JPE:
            print_jpe(get_next_word());
            break;
        case XCHG:
            print_xchg();
            break;
        case CPE:
            print_cpe(get_next_word());
            break;
        case UNUSED_CALL_2:
            print_unused_call(get_next_word());
            break;
        case XRI:
            print_xri(get_next_byte());
            break;
        case RST_5:
            print_rst(5);
            break;
        case RP:
            print_rp();
            break;
        case POS_PSW:
            print_pop("PSW");
            break;
        case JP:
            print_jp(get_next_word());
            break;
        case DI:
            print_di();
            break;
        case CP:
            print_cp(get_next_word());
            break;
        case PUSH_PSW:
            print_push("PSW");
            break;
        case ORI:
            print_ori(get_next_byte());
            break;
        case RST_6:
            print_rst(6);
            break;
        case RM:
            print_rm();
            break;
        case SPHL:
            print_sphl();
            break;
        case JM:
            print_jm(get_next_word());
            break;
        case EI:
            print_ei();
            break;
        case CM:
            print_cm(get_next_word());
            break;
        case UNUSED_CALL_3:
            print_unused_call(get_next_word());
            break;
        case CPI:
            print_cpi(get_next_byte());
            break;
        case RST_7:
            print_rst(7);
            break;
        default:
            throw emu::util::exceptions::UnrecognizedOpcodeException(m_opcode);
        }

        std::cout << "\n";
    }

    NextByte Disassembler8080::get_next_byte() {
        return {
                .farg = m_memory[m_pc++]
        };
    }

    NextWord Disassembler8080::get_next_word() {
        return {
                .farg = m_memory[m_pc++],
                .sarg = m_memory[m_pc++]
        };
    }
}
