#include "disassembler.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include <iostream>
#include <string>

namespace emu::i8080 {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::string::hexify_wo_0x;

Disassembler::Disassembler(EmulatorMemory<u16, u8>& memory, std::ostream& ostream)
    : m_memory(memory)
    , m_memory_size(memory.size())
    , m_pc(0)
    , m_opcode(0)
    , m_ostream(ostream)
{
}

void Disassembler::disassemble()
{
    while (m_pc < m_memory_size) {
        print_next_instruction();
    }
}

void Disassembler::print_next_instruction()
{
    m_ostream << hexify_wo_0x(m_pc, 4) << "\t\t";

    m_opcode = get_next_byte().farg;

    switch (m_opcode) {
    case NOP:
        print_nop(m_ostream);
        break;
    case LXI_B:
        print_lxi(m_ostream, "B", get_next_word());
        break;
    case STAX_B:
        print_stax(m_ostream, "B");
        break;
    case INX_B:
        print_inx(m_ostream, "B");
        break;
    case INR_B:
        print_inr(m_ostream, "B");
        break;
    case DCR_B:
        print_dcr(m_ostream, "B");
        break;
    case MVI_B:
        print_mvi(m_ostream, "B", get_next_byte());
        break;
    case RLC_B:
        print_rlc(m_ostream, "B");
        break;
    case UNUSED_NOP_1:
        print_unused_nop(m_ostream);
        break;
    case DAD_B:
        print_dad(m_ostream, "B");
        break;
    case LDAX_B:
        print_ldax(m_ostream, "B");
        break;
    case DCX_B:
        print_dcx(m_ostream, "B");
        break;
    case INR_C:
        print_inr(m_ostream, "C");
        break;
    case DCR_C:
        print_dcr(m_ostream, "C");
        break;
    case MVI_C:
        print_mvi(m_ostream, "C", get_next_byte());
        break;
    case RRC:
        print_rrc(m_ostream);
        break;
    case UNUSED_NOP_2:
        print_unused_nop(m_ostream);
        break;
    case LXI_D:
        print_lxi(m_ostream, "D", get_next_word());
        break;
    case STAX_D:
        print_stax(m_ostream, "D");
        break;
    case INX_D:
        print_inx(m_ostream, "D");
        break;
    case INR_D:
        print_inr(m_ostream, "D");
        break;
    case DCR_D:
        print_dcr(m_ostream, "D");
        break;
    case MVI_D:
        print_mvi(m_ostream, "D", get_next_byte());
        break;
    case RAL:
        print_ral(m_ostream);
        break;
    case UNUSED_NOP_3:
        print_unused_nop(m_ostream);
        break;
    case DAD_D:
        print_dad(m_ostream, "D");
        break;
    case LDAX_D:
        print_ldax(m_ostream, "D");
        break;
    case DCX_D:
        print_dcx(m_ostream, "D");
        break;
    case INR_E:
        print_inr(m_ostream, "E");
        break;
    case DCR_E:
        print_dcr(m_ostream, "E");
        break;
    case MVI_E:
        print_mvi(m_ostream, "E", get_next_byte());
        break;
    case RAR:
        print_rar(m_ostream);
        break;
    case UNUSED_NOP_4:
        print_unused_nop(m_ostream);
        break;
    case LXI_H:
        print_lxi(m_ostream, "H", get_next_word());
        break;
    case SHLD:
        print_shld(m_ostream, get_next_word());
        break;
    case INX_H:
        print_inx(m_ostream, "H");
        break;
    case INR_H:
        print_inr(m_ostream, "H");
        break;
    case DCR_H:
        print_dcr(m_ostream, "H");
        break;
    case MVI_H:
        print_mvi(m_ostream, "H", get_next_byte());
        break;
    case DAA:
        print_daa(m_ostream);
        break;
    case UNUSED_NOP_5:
        print_unused_nop(m_ostream);
        break;
    case DAD_H:
        print_dad(m_ostream, "H");
        break;
    case LHLD:
        print_lhld(m_ostream, get_next_word());
        break;
    case DCX_H:
        print_dcx(m_ostream, "H");
        break;
    case INR_L:
        print_inr(m_ostream, "L");
        break;
    case DCR_L:
        print_dcr(m_ostream, "L");
        break;
    case MVI_L:
        print_mvi(m_ostream, "L", get_next_byte());
        break;
    case CMA:
        print_cma(m_ostream);
        break;
    case UNUSED_NOP_6:
        print_unused_nop(m_ostream);
        break;
    case LXI_SP:
        print_lxi(m_ostream, "SP", get_next_word());
        break;
    case STA:
        print_sta(m_ostream, get_next_word());
        break;
    case INX_SP:
        print_inx(m_ostream, "SP");
        break;
    case INR_M:
        print_inr(m_ostream, "M");
        break;
    case DCR_M:
        print_dcr(m_ostream, "M");
        break;
    case MVI_M:
        print_mvi(m_ostream, "M", get_next_byte());
        break;
    case STC:
        print_stc(m_ostream);
        break;
    case UNUSED_NOP_7:
        print_unused_nop(m_ostream);
        break;
    case DAD_SP:
        print_dad(m_ostream, "SP");
        break;
    case LDA:
        print_lda(m_ostream, get_next_word());
        break;
    case DCX_SP:
        print_dcx(m_ostream, "SP");
        break;
    case INR_A:
        print_inr(m_ostream, "A");
        break;
    case DCR_A:
        print_dcr(m_ostream, "A");
        break;
    case MVI_A:
        print_mvi(m_ostream, "A", get_next_byte());
        break;
    case CMC:
        print_cmc(m_ostream);
        break;
    case MOV_B_B:
        print_mov(m_ostream, "B", "B");
        break;
    case MOV_B_C:
        print_mov(m_ostream, "B", "C");
        break;
    case MOV_B_D:
        print_mov(m_ostream, "B", "D");
        break;
    case MOV_B_E:
        print_mov(m_ostream, "B", "E");
        break;
    case MOV_B_H:
        print_mov(m_ostream, "B", "H");
        break;
    case MOV_B_L:
        print_mov(m_ostream, "B", "L");
        break;
    case MOV_B_M:
        print_mov(m_ostream, "B", "M");
        break;
    case MOV_B_A:
        print_mov(m_ostream, "B", "A");
        break;
    case MOV_C_B:
        print_mov(m_ostream, "C", "B");
        break;
    case MOV_C_C:
        print_mov(m_ostream, "C", "C");
        break;
    case MOV_C_D:
        print_mov(m_ostream, "C", "D");
        break;
    case MOV_C_E:
        print_mov(m_ostream, "C", "E");
        break;
    case MOV_C_H:
        print_mov(m_ostream, "C", "H");
        break;
    case MOV_C_L:
        print_mov(m_ostream, "C", "L");
        break;
    case MOV_C_M:
        print_mov(m_ostream, "C", "M");
        break;
    case MOV_C_A:
        print_mov(m_ostream, "C", "A");
        break;
    case MOV_D_B:
        print_mov(m_ostream, "D", "B");
        break;
    case MOV_D_C:
        print_mov(m_ostream, "D", "C");
        break;
    case MOV_D_D:
        print_mov(m_ostream, "D", "D");
        break;
    case MOV_D_E:
        print_mov(m_ostream, "D", "E");
        break;
    case MOV_D_H:
        print_mov(m_ostream, "D", "H");
        break;
    case MOV_D_L:
        print_mov(m_ostream, "D", "L");
        break;
    case MOV_D_M:
        print_mov(m_ostream, "D", "M");
        break;
    case MOV_D_A:
        print_mov(m_ostream, "D", "A");
        break;
    case MOV_E_B:
        print_mov(m_ostream, "E", "B");
        break;
    case MOV_E_C:
        print_mov(m_ostream, "E", "C");
        break;
    case MOV_E_D:
        print_mov(m_ostream, "E", "D");
        break;
    case MOV_E_E:
        print_mov(m_ostream, "E", "E");
        break;
    case MOV_E_H:
        print_mov(m_ostream, "E", "H");
        break;
    case MOV_E_L:
        print_mov(m_ostream, "E", "L");
        break;
    case MOV_E_M:
        print_mov(m_ostream, "E", "M");
        break;
    case MOV_E_A:
        print_mov(m_ostream, "E", "A");
        break;
    case MOV_H_B:
        print_mov(m_ostream, "H", "B");
        break;
    case MOV_H_C:
        print_mov(m_ostream, "H", "C");
        break;
    case MOV_H_D:
        print_mov(m_ostream, "H", "D");
        break;
    case MOV_H_E:
        print_mov(m_ostream, "H", "E");
        break;
    case MOV_H_H:
        print_mov(m_ostream, "H", "H");
        break;
    case MOV_H_L:
        print_mov(m_ostream, "H", "L");
        break;
    case MOV_H_M:
        print_mov(m_ostream, "H", "M");
        break;
    case MOV_H_A:
        print_mov(m_ostream, "H", "A");
        break;
    case MOV_L_B:
        print_mov(m_ostream, "L", "B");
        break;
    case MOV_L_C:
        print_mov(m_ostream, "L", "C");
        break;
    case MOV_L_D:
        print_mov(m_ostream, "L", "D");
        break;
    case MOV_L_E:
        print_mov(m_ostream, "L", "E");
        break;
    case MOV_L_H:
        print_mov(m_ostream, "L", "H");
        break;
    case MOV_L_L:
        print_mov(m_ostream, "L", "L");
        break;
    case MOV_L_M:
        print_mov(m_ostream, "L", "M");
        break;
    case MOV_L_A:
        print_mov(m_ostream, "L", "A");
        break;
    case MOV_M_B:
        print_mov(m_ostream, "M", "B");
        break;
    case MOV_M_C:
        print_mov(m_ostream, "M", "C");
        break;
    case MOV_M_D:
        print_mov(m_ostream, "M", "D");
        break;
    case MOV_M_E:
        print_mov(m_ostream, "M", "E");
        break;
    case MOV_M_H:
        print_mov(m_ostream, "M", "H");
        break;
    case MOV_M_L:
        print_mov(m_ostream, "M", "L");
        break;
    case HLT:
        print_hlt(m_ostream);
        break;
    case MOV_M_A:
        print_mov(m_ostream, "M", "A");
        break;
    case MOV_A_B:
        print_mov(m_ostream, "A", "B");
        break;
    case MOV_A_C:
        print_mov(m_ostream, "A", "C");
        break;
    case MOV_A_D:
        print_mov(m_ostream, "A", "D");
        break;
    case MOV_A_E:
        print_mov(m_ostream, "A", "E");
        break;
    case MOV_A_H:
        print_mov(m_ostream, "A", "H");
        break;
    case MOV_A_L:
        print_mov(m_ostream, "A", "L");
        break;
    case MOV_A_M:
        print_mov(m_ostream, "A", "M");
        break;
    case MOV_A_A:
        print_mov(m_ostream, "A", "A");
        break;
    case ADD_B:
        print_add(m_ostream, "B");
        break;
    case ADD_C:
        print_add(m_ostream, "C");
        break;
    case ADD_D:
        print_add(m_ostream, "D");
        break;
    case ADD_E:
        print_add(m_ostream, "E");
        break;
    case ADD_H:
        print_add(m_ostream, "H");
        break;
    case ADD_L:
        print_add(m_ostream, "L");
        break;
    case ADD_M:
        print_add(m_ostream, "M");
        break;
    case ADD_A:
        print_add(m_ostream, "A");
        break;
    case ADC_B:
        print_adc(m_ostream, "B");
        break;
    case ADC_C:
        print_adc(m_ostream, "C");
        break;
    case ADC_D:
        print_adc(m_ostream, "D");
        break;
    case ADC_E:
        print_adc(m_ostream, "E");
        break;
    case ADC_H:
        print_adc(m_ostream, "H");
        break;
    case ADC_L:
        print_adc(m_ostream, "L");
        break;
    case ADC_M:
        print_adc(m_ostream, "M");
        break;
    case ADC_A:
        print_adc(m_ostream, "A");
        break;
    case SUB_B:
        print_sub(m_ostream, "B");
        break;
    case SUB_C:
        print_sub(m_ostream, "C");
        break;
    case SUB_D:
        print_sub(m_ostream, "D");
        break;
    case SUB_E:
        print_sub(m_ostream, "E");
        break;
    case SUB_H:
        print_sub(m_ostream, "H");
        break;
    case SUB_L:
        print_sub(m_ostream, "L");
        break;
    case SUB_M:
        print_sub(m_ostream, "M");
        break;
    case SUB_A:
        print_sub(m_ostream, "A");
        break;
    case SBB_B:
        print_sbb(m_ostream, "B");
        break;
    case SBB_C:
        print_sbb(m_ostream, "C");
        break;
    case SBB_D:
        print_sbb(m_ostream, "D");
        break;
    case SBB_E:
        print_sbb(m_ostream, "E");
        break;
    case SBB_H:
        print_sbb(m_ostream, "H");
        break;
    case SBB_L:
        print_sbb(m_ostream, "L");
        break;
    case SBB_M:
        print_sbb(m_ostream, "M");
        break;
    case SBB_A:
        print_sbb(m_ostream, "A");
        break;
    case ANA_B:
        print_ana(m_ostream, "B");
        break;
    case ANA_C:
        print_ana(m_ostream, "C");
        break;
    case ANA_D:
        print_ana(m_ostream, "D");
        break;
    case ANA_E:
        print_ana(m_ostream, "E");
        break;
    case ANA_H:
        print_ana(m_ostream, "H");
        break;
    case ANA_L:
        print_ana(m_ostream, "L");
        break;
    case ANA_M:
        print_ana(m_ostream, "M");
        break;
    case ANA_A:
        print_ana(m_ostream, "A");
        break;
    case XRA_B:
        print_xra(m_ostream, "B");
        break;
    case XRA_C:
        print_xra(m_ostream, "C");
        break;
    case XRA_D:
        print_xra(m_ostream, "D");
        break;
    case XRA_E:
        print_xra(m_ostream, "E");
        break;
    case XRA_H:
        print_xra(m_ostream, "H");
        break;
    case XRA_L:
        print_xra(m_ostream, "L");
        break;
    case XRA_M:
        print_xra(m_ostream, "M");
        break;
    case XRA_A:
        print_xra(m_ostream, "A");
        break;
    case ORA_B:
        print_ora(m_ostream, "B");
        break;
    case ORA_C:
        print_ora(m_ostream, "C");
        break;
    case ORA_D:
        print_ora(m_ostream, "D");
        break;
    case ORA_E:
        print_ora(m_ostream, "E");
        break;
    case ORA_H:
        print_ora(m_ostream, "H");
        break;
    case ORA_L:
        print_ora(m_ostream, "L");
        break;
    case ORA_M:
        print_ora(m_ostream, "M");
        break;
    case ORA_A:
        print_ora(m_ostream, "A");
        break;
    case CMP_B:
        print_cmp(m_ostream, "B");
        break;
    case CMP_C:
        print_cmp(m_ostream, "C");
        break;
    case CMP_D:
        print_cmp(m_ostream, "D");
        break;
    case CMP_E:
        print_cmp(m_ostream, "E");
        break;
    case CMP_H:
        print_cmp(m_ostream, "H");
        break;
    case CMP_L:
        print_cmp(m_ostream, "L");
        break;
    case CMP_M:
        print_cmp(m_ostream, "M");
        break;
    case CMP_A:
        print_cmp(m_ostream, "A");
        break;
    case RNZ:
        print_rnz(m_ostream);
        break;
    case POP_B:
        print_pop(m_ostream, "B");
        break;
    case JNZ:
        print_jnz(m_ostream, get_next_word());
        break;
    case JMP:
        print_jmp(m_ostream, get_next_word());
        break;
    case CNZ:
        print_cnz(m_ostream, get_next_word());
        break;
    case PUSH_B:
        print_push(m_ostream, "B");
        break;
    case ADI:
        print_adi(m_ostream, get_next_byte());
        break;
    case RST_0:
        print_rst(m_ostream, 0);
        break;
    case RZ:
        print_rz(m_ostream);
        break;
    case RET:
        print_ret(m_ostream);
        break;
    case JZ:
        print_jz(m_ostream, get_next_word());
        break;
    case UNUSED_JMP_1:
        print_unused_jmp(m_ostream, get_next_word());
        break;
    case CZ:
        print_cz(m_ostream, get_next_word());
        break;
    case CALL:
        print_call(m_ostream, get_next_word());
        break;
    case ACI:
        print_aci(m_ostream, get_next_byte());
        break;
    case RST_1:
        print_rst(m_ostream, 1);
        break;
    case RNC:
        print_rnc(m_ostream);
        break;
    case POP_D:
        print_pop(m_ostream, "D");
        break;
    case JNC:
        print_jnc(m_ostream, get_next_word());
        break;
    case OUT:
        print_out(m_ostream, get_next_byte());
        break;
    case CNC:
        print_cnc(m_ostream, get_next_word());
        break;
    case PUSH_D:
        print_push(m_ostream, "D");
        break;
    case SUI:
        print_sui(m_ostream, get_next_byte());
        break;
    case RST_2:
        print_rst(m_ostream, 2);
        break;
    case RC:
        print_rc(m_ostream);
        break;
    case UNUSED_RET_1:
        print_unused_ret(m_ostream);
        break;
    case JC:
        print_jc(m_ostream, get_next_word());
        break;
    case IN:
        print_in(m_ostream, get_next_byte());
        break;
    case CC:
        print_cc(m_ostream, get_next_word());
        break;
    case UNUSED_CALL_1:
        print_unused_call(m_ostream, get_next_word());
        break;
    case SBI:
        print_sbi(m_ostream, get_next_byte());
        break;
    case RST_3:
        print_rst(m_ostream, 3);
        break;
    case RPO:
        print_rpo(m_ostream);
        break;
    case POP_H:
        print_pop(m_ostream, "H");
        break;
    case JPO:
        print_jpo(m_ostream, get_next_word());
        break;
    case XTHL:
        print_xthl(m_ostream);
        break;
    case CPO:
        print_cpo(m_ostream, get_next_word());
        break;
    case PUSH_H:
        print_push(m_ostream, "H");
        break;
    case ANI:
        print_ani(m_ostream, get_next_byte());
        break;
    case RST_4:
        print_rst(m_ostream, 4);
        break;
    case RPE:
        print_rpe(m_ostream);
        break;
    case PCHL:
        print_pchl(m_ostream);
        break;
    case JPE:
        print_jpe(m_ostream, get_next_word());
        break;
    case XCHG:
        print_xchg(m_ostream);
        break;
    case CPE:
        print_cpe(m_ostream, get_next_word());
        break;
    case UNUSED_CALL_2:
        print_unused_call(m_ostream, get_next_word());
        break;
    case XRI:
        print_xri(m_ostream, get_next_byte());
        break;
    case RST_5:
        print_rst(m_ostream, 5);
        break;
    case RP:
        print_rp(m_ostream);
        break;
    case POP_PSW:
        print_pop(m_ostream, "PSW");
        break;
    case JP:
        print_jp(m_ostream, get_next_word());
        break;
    case DI:
        print_di(m_ostream);
        break;
    case CP:
        print_cp(m_ostream, get_next_word());
        break;
    case PUSH_PSW:
        print_push(m_ostream, "PSW");
        break;
    case ORI:
        print_ori(m_ostream, get_next_byte());
        break;
    case RST_6:
        print_rst(m_ostream, 6);
        break;
    case RM:
        print_rm(m_ostream);
        break;
    case SPHL:
        print_sphl(m_ostream);
        break;
    case JM:
        print_jm(m_ostream, get_next_word());
        break;
    case EI:
        print_ei(m_ostream);
        break;
    case CM:
        print_cm(m_ostream, get_next_word());
        break;
    case UNUSED_CALL_3:
        print_unused_call(m_ostream, get_next_word());
        break;
    case CPI:
        print_cpi(m_ostream, get_next_byte());
        break;
    case RST_7:
        print_rst(m_ostream, 7);
        break;
    default:
        throw UnrecognizedOpcodeException(m_opcode);
    }

    m_ostream << "\n";
}

NextByte Disassembler::get_next_byte()
{
    return {
        .farg = m_memory.read(m_pc++)
    };
}

NextWord Disassembler::get_next_word()
{
    return {
        .farg = m_memory.read(m_pc++),
        .sarg = m_memory.read(m_pc++)
    };
}
}
