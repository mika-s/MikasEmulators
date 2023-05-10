#include "disassembler.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::util::string::hexify;
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

    if (0x0104 <= m_pc && m_pc <= 0x014f + 1) {
        print_cartridge_header();
        return;
    }

    switch (m_opcode) {
    case NOP:
        print_nop(m_ostream);
        break;
    case LD_BC_nn:
        print_ld_dd_nn(m_ostream, "BC", get_next_word());
        break;
    case LD_MBC_A:
        print_ld(m_ostream, "(BC)", "A");
        break;
    case INC_BC:
        print_inc(m_ostream, "BC");
        break;
    case INC_B:
        print_inc(m_ostream, "B");
        break;
    case DEC_B:
        print_dec(m_ostream, "B");
        break;
    case LD_B_n:
        print_ld(m_ostream, "B", get_next_byte());
        break;
    case RLCA:
        print_rlca(m_ostream);
        break;
    case LD_Mnn_SP:
        print_ld(m_ostream, get_next_word(), "SP");
        break;
    case ADD_HL_BC:
        print_add(m_ostream, "HL", "BC");
        break;
    case LD_A_MBC:
        print_ld(m_ostream, "A", "(BC)");
        break;
    case DEC_BC:
        print_dec(m_ostream, "BC");
        break;
    case INC_C:
        print_inc(m_ostream, "C");
        break;
    case DEC_C:
        print_dec(m_ostream, "C");
        break;
    case LD_C_n:
        print_ld(m_ostream, "C", get_next_byte());
        break;
    case RRCA:
        print_rrca(m_ostream);
        break;
    case STOP_0:
        print_stop(m_ostream);
        break;
    case LD_DE_nn:
        print_ld_dd_nn(m_ostream, "DE", get_next_word());
        break;
    case LD_MDE_A:
        print_ld(m_ostream, "(DE)", "A");
        break;
    case INC_DE:
        print_inc(m_ostream, "DE");
        break;
    case INC_D:
        print_inc(m_ostream, "D");
        break;
    case DEC_D:
        print_dec(m_ostream, "D");
        break;
    case LD_D_n:
        print_ld(m_ostream, "D", get_next_byte());
        break;
    case RLA:
        print_rla(m_ostream);
        break;
    case JR_e:
        print_jr(m_ostream, get_next_byte());
        break;
    case ADD_HL_DE:
        print_add(m_ostream, "HL", "DE");
        break;
    case LD_A_MDE:
        print_ld(m_ostream, "A", "(DE)");
        break;
    case DEC_DE:
        print_dec(m_ostream, "D");
        break;
    case INC_E:
        print_inc(m_ostream, "E");
        break;
    case DEC_E:
        print_dec(m_ostream, "E");
        break;
    case LD_E_n:
        print_ld(m_ostream, "E", get_next_byte());
        break;
    case RRA:
        print_rra(m_ostream);
        break;
    case JR_NZ_e:
        print_jr(m_ostream, "NZ", get_next_byte());
        break;
    case LD_HL_nn:
        print_ld_dd_nn(m_ostream, "HL", get_next_word());
        break;
    case LD_MHLp_A:
        print_ld_Mnn_dd(m_ostream, get_next_word(), "HL"); // TODO
        break;
    case INC_HL:
        print_inc(m_ostream, "HL");
        break;
    case INC_H:
        print_inc(m_ostream, "H");
        break;
    case DEC_H:
        print_dec(m_ostream, "H");
        break;
    case LD_H_n:
        print_ld(m_ostream, "H", get_next_byte());
        break;
    case DAA:
        print_daa(m_ostream);
        break;
    case JR_Z_e:
        print_jr(m_ostream, "Z", get_next_byte());
        break;
    case ADD_HL_HL:
        print_add(m_ostream, "HL", "HL");
        break;
    case LD_A_MHLp:
        print_ld(m_ostream, "HL", get_next_word());
        break;
    case DEC_HL:
        print_dec(m_ostream, "HL");
        break;
    case INC_L:
        print_inc(m_ostream, "L");
        break;
    case DEC_L:
        print_dec(m_ostream, "L");
        break;
    case LD_L_n:
        print_ld(m_ostream, "L", get_next_byte());
        break;
    case CPL:
        print_cpl(m_ostream);
        break;
    case JR_NC_e:
        print_jr(m_ostream, "NC", get_next_byte());
        break;
    case LD_SP_nn:
        print_ld_dd_nn(m_ostream, "SP", get_next_word());
        break;
    case LH_MHLm_A:
        print_ld_Mnn_dd(m_ostream, get_next_word(), "A"); // TODO
        break;
    case INC_SP:
        print_inc(m_ostream, "SP");
        break;
    case INC_MHL:
        print_inc(m_ostream, "(HL)");
        break;
    case DEC_MHL:
        print_dec(m_ostream, "(HL)");
        break;
    case LD_MHL_n:
        print_ld(m_ostream, "(HL)", get_next_byte());
        break;
    case SCF:
        print_scf(m_ostream);
        break;
    case JR_C_e:
        print_jr(m_ostream, "C", get_next_byte());
        break;
    case ADD_HL_SP:
        print_add(m_ostream, "HL", "SP");
        break;
    case LD_A_MHLm:
        print_ld(m_ostream, "A", get_next_word());
        break;
    case DEC_SP:
        print_dec(m_ostream, "SP");
        break;
    case INC_A:
        print_inc(m_ostream, "A");
        break;
    case DEC_A:
        print_dec(m_ostream, "A");
        break;
    case LD_A_n:
        print_ld(m_ostream, "A", get_next_byte());
        break;
    case CCF:
        print_ccf(m_ostream);
        break;
    case LD_B_B:
        print_ld(m_ostream, "B", "B");
        break;
    case LD_B_C:
        print_ld(m_ostream, "B", "C");
        break;
    case LD_B_D:
        print_ld(m_ostream, "B", "D");
        break;
    case LD_B_E:
        print_ld(m_ostream, "B", "E");
        break;
    case LD_B_H:
        print_ld(m_ostream, "B", "H");
        break;
    case LD_B_L:
        print_ld(m_ostream, "B", "L");
        break;
    case LD_B_MHL:
        print_ld(m_ostream, "B", "(HL)");
        break;
    case LD_B_A:
        print_ld(m_ostream, "B", "A");
        break;
    case LD_C_B:
        print_ld(m_ostream, "C", "B");
        break;
    case LD_C_C:
        print_ld(m_ostream, "C", "C");
        break;
    case LD_C_D:
        print_ld(m_ostream, "C", "D");
        break;
    case LD_C_E:
        print_ld(m_ostream, "C", "E");
        break;
    case LD_C_H:
        print_ld(m_ostream, "C", "H");
        break;
    case LD_C_L:
        print_ld(m_ostream, "C", "L");
        break;
    case LD_C_MHL:
        print_ld(m_ostream, "C", "(HL)");
        break;
    case LD_C_A:
        print_ld(m_ostream, "C", "A");
        break;
    case LD_D_B:
        print_ld(m_ostream, "D", "B");
        break;
    case LD_D_C:
        print_ld(m_ostream, "D", "C");
        break;
    case LD_D_D:
        print_ld(m_ostream, "D", "D");
        break;
    case LD_D_E:
        print_ld(m_ostream, "D", "E");
        break;
    case LD_D_H:
        print_ld(m_ostream, "D", "H");
        break;
    case LD_D_L:
        print_ld(m_ostream, "D", "L");
        break;
    case LD_D_MHL:
        print_ld(m_ostream, "D", "(HL)");
        break;
    case LD_D_A:
        print_ld(m_ostream, "D", "A");
        break;
    case LD_E_B:
        print_ld(m_ostream, "E", "B");
        break;
    case LD_E_C:
        print_ld(m_ostream, "E", "C");
        break;
    case LD_E_D:
        print_ld(m_ostream, "E", "D");
        break;
    case LD_E_E:
        print_ld(m_ostream, "E", "E");
        break;
    case LD_E_H:
        print_ld(m_ostream, "E", "H");
        break;
    case LD_E_L:
        print_ld(m_ostream, "E", "L");
        break;
    case LD_E_MHL:
        print_ld(m_ostream, "E", "(HL)");
        break;
    case LD_E_A:
        print_ld(m_ostream, "E", "A");
        break;
    case LD_H_B:
        print_ld(m_ostream, "H", "B");
        break;
    case LD_H_C:
        print_ld(m_ostream, "H", "C");
        break;
    case LD_H_D:
        print_ld(m_ostream, "H", "D");
        break;
    case LD_H_E:
        print_ld(m_ostream, "H", "E");
        break;
    case LD_H_H:
        print_ld(m_ostream, "H", "H");
        break;
    case LD_H_L:
        print_ld(m_ostream, "H", "L");
        break;
    case LD_H_MHL:
        print_ld(m_ostream, "H", "(HL)");
        break;
    case LD_H_A:
        print_ld(m_ostream, "H", "A");
        break;
    case LD_L_B:
        print_ld(m_ostream, "L", "B");
        break;
    case LD_L_C:
        print_ld(m_ostream, "L", "C");
        break;
    case LD_L_D:
        print_ld(m_ostream, "L", "D");
        break;
    case LD_L_E:
        print_ld(m_ostream, "L", "E");
        break;
    case LD_L_H:
        print_ld(m_ostream, "L", "H");
        break;
    case LD_L_L:
        print_ld(m_ostream, "L", "L");
        break;
    case LD_L_MHL:
        print_ld(m_ostream, "L", "(HL)");
        break;
    case LD_L_A:
        print_ld(m_ostream, "L", "A");
        break;
    case LD_MHL_B:
        print_ld(m_ostream, "(HL)", "B");
        break;
    case LD_MHL_C:
        print_ld(m_ostream, "(HL)", "C");
        break;
    case LD_MHL_D:
        print_ld(m_ostream, "(HL)", "D");
        break;
    case LD_MHL_E:
        print_ld(m_ostream, "(HL)", "E");
        break;
    case LD_MHL_H:
        print_ld(m_ostream, "(HL)", "H");
        break;
    case LD_MHL_L:
        print_ld(m_ostream, "(HL)", "L");
        break;
    case HALT:
        print_halt(m_ostream);
        break;
    case LD_MHL_A:
        print_ld(m_ostream, "(HL)", "A");
        break;
    case LD_A_B:
        print_ld(m_ostream, "A", "B");
        break;
    case LD_A_C:
        print_ld(m_ostream, "A", "C");
        break;
    case LD_A_D:
        print_ld(m_ostream, "A", "D");
        break;
    case LD_A_E:
        print_ld(m_ostream, "A", "E");
        break;
    case LD_A_H:
        print_ld(m_ostream, "A", "H");
        break;
    case LD_A_L:
        print_ld(m_ostream, "A", "L");
        break;
    case LD_A_MHL:
        print_ld(m_ostream, "A", "(HL)");
        break;
    case LD_A_A:
        print_ld(m_ostream, "A", "A");
        break;
    case ADD_A_B:
        print_add(m_ostream, "A", "B");
        break;
    case ADD_A_C:
        print_add(m_ostream, "A", "C");
        break;
    case ADD_A_D:
        print_add(m_ostream, "A", "D");
        break;
    case ADD_A_E:
        print_add(m_ostream, "A", "E");
        break;
    case ADD_A_H:
        print_add(m_ostream, "A", "H");
        break;
    case ADD_A_L:
        print_add(m_ostream, "A", "L");
        break;
    case ADD_A_MHL:
        print_add(m_ostream, "A", "(HL)");
        break;
    case ADD_A_A:
        print_add(m_ostream, "A", "A");
        break;
    case ADC_A_B:
        print_adc_r_s(m_ostream, "A", "B");
        break;
    case ADC_A_C:
        print_adc_r_s(m_ostream, "A", "C");
        break;
    case ADC_A_D:
        print_adc_r_s(m_ostream, "A", "D");
        break;
    case ADC_A_E:
        print_adc_r_s(m_ostream, "A", "E");
        break;
    case ADC_A_H:
        print_adc_r_s(m_ostream, "A", "H");
        break;
    case ADC_A_L:
        print_adc_r_s(m_ostream, "A", "L");
        break;
    case ADC_A_MHL:
        print_adc_r_s(m_ostream, "A", "(HL)");
        break;
    case ADC_A_A:
        print_adc_r_s(m_ostream, "A", "A");
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
    case SUB_MHL:
        print_sub(m_ostream, "(HL)");
        break;
    case SUB_A:
        print_sub(m_ostream, "A");
        break;
    case SBC_A_B:
        print_sbc_r_s(m_ostream, "A", "B");
        break;
    case SBC_A_C:
        print_sbc_r_s(m_ostream, "A", "C");
        break;
    case SBC_A_D:
        print_sbc_r_s(m_ostream, "A", "D");
        break;
    case SBC_A_E:
        print_sbc_r_s(m_ostream, "A", "E");
        break;
    case SBC_A_H:
        print_sbc_r_s(m_ostream, "A", "H");
        break;
    case SBC_A_L:
        print_sbc_r_s(m_ostream, "A", "L");
        break;
    case SBC_A_MHL:
        print_sbc_r_s(m_ostream, "A", "(HL)");
        break;
    case SBC_A_A:
        print_sbc_r_s(m_ostream, "A", "A");
        break;
    case AND_B:
        print_and_r(m_ostream, "B");
        break;
    case AND_C:
        print_and_r(m_ostream, "C");
        break;
    case AND_D:
        print_and_r(m_ostream, "D");
        break;
    case AND_E:
        print_and_r(m_ostream, "E");
        break;
    case AND_H:
        print_and_r(m_ostream, "H");
        break;
    case AND_L:
        print_and_r(m_ostream, "L");
        break;
    case AND_MHL:
        print_and_r(m_ostream, "(HL)");
        break;
    case AND_A:
        print_and_r(m_ostream, "A");
        break;
    case XOR_B:
        print_xor_r(m_ostream, "B");
        break;
    case XOR_C:
        print_xor_r(m_ostream, "C");
        break;
    case XOR_D:
        print_xor_r(m_ostream, "D");
        break;
    case XOR_E:
        print_xor_r(m_ostream, "E");
        break;
    case XOR_H:
        print_xor_r(m_ostream, "H");
        break;
    case XOR_L:
        print_xor_r(m_ostream, "L");
        break;
    case XOR_MHL:
        print_xor_r(m_ostream, "(HL)");
        break;
    case XOR_A:
        print_xor_r(m_ostream, "A");
        break;
    case OR_B:
        print_or_r(m_ostream, "B");
        break;
    case OR_C:
        print_or_r(m_ostream, "C");
        break;
    case OR_D:
        print_or_r(m_ostream, "D");
        break;
    case OR_E:
        print_or_r(m_ostream, "E");
        break;
    case OR_H:
        print_or_r(m_ostream, "H");
        break;
    case OR_L:
        print_or_r(m_ostream, "L");
        break;
    case OR_MHL:
        print_or_r(m_ostream, "(HL)");
        break;
    case OR_A:
        print_or_r(m_ostream, "A");
        break;
    case CP_B:
        print_cp(m_ostream, "B");
        break;
    case CP_C:
        print_cp(m_ostream, "C");
        break;
    case CP_D:
        print_cp(m_ostream, "D");
        break;
    case CP_E:
        print_cp(m_ostream, "E");
        break;
    case CP_H:
        print_cp(m_ostream, "H");
        break;
    case CP_L:
        print_cp(m_ostream, "L");
        break;
    case CP_MHL:
        print_cp(m_ostream, "(HL)");
        break;
    case CP_A:
        print_cp(m_ostream, "A");
        break;
    case RET_NZ:
        print_ret(m_ostream, "NZ");
        break;
    case POP_BC:
        print_pop(m_ostream, "BC");
        break;
    case JP_NZ:
        print_jp_cc_nn(m_ostream, get_next_word(), "NZ");
        break;
    case JP:
        print_jp_nn(m_ostream, get_next_word());
        break;
    case CALL_NZ:
        print_call_cc_nn(m_ostream, get_next_word(), "NZ");
        break;
    case PUSH_BC:
        print_push(m_ostream, "BC");
        break;
    case ADD_A_n:
        print_add_r_n(m_ostream, "A", get_next_byte());
        break;
    case RST_0:
        print_rst(m_ostream, 0);
        break;
    case RET_Z:
        print_ret(m_ostream, "Z");
        break;
    case RET:
        print_ret(m_ostream);
        break;
    case JP_Z:
        print_jp_cc_nn(m_ostream, get_next_word(), "Z");
        break;
    case BITS:
        print_next_bits_instruction(get_next_byte().farg);
        break;
    case CALL_Z:
        print_call_cc_nn(m_ostream, get_next_word(), "Z");
        break;
    case CALL:
        print_call_nn(m_ostream, get_next_word());
        break;
    case ADC_A_n:
        print_adc_r_n(m_ostream, "A", get_next_byte());
        break;
    case RST_1:
        print_rst(m_ostream, 1);
        break;
    case RET_NC:
        print_ret(m_ostream, "NZ");
        break;
    case POP_DE:
        print_pop(m_ostream, "DE");
        break;
    case JP_NC:
        print_jp_cc_nn(m_ostream, get_next_word(), "NC");
        break;
    case CALL_NC:
        print_call_cc_nn(m_ostream, get_next_word(), "NC");
        break;
    case PUSH_DE:
        print_push(m_ostream, "DE");
        break;
    case SUB_n:
        print_sub(m_ostream, get_next_byte());
        break;
    case RST_2:
        print_rst(m_ostream, 2);
        break;
    case RET_C:
        print_ret(m_ostream, "C");
        break;
    case RETI:
        print_reti(m_ostream);
        break;
    case JP_C:
        print_jp_cc_nn(m_ostream, get_next_word(), "C");
        break;
    case CALL_C:
        print_call_cc_nn(m_ostream, get_next_word(), "C");
        break;
    case SBC_A_n:
        print_sbc_r_n(m_ostream, "A", get_next_byte());
        break;
    case RST_3:
        print_rst(m_ostream, 3);
        break;
    case LDH_Mn_A:
        print_ldh_Mn_A(m_ostream, get_next_byte());
        break;
    case POP_HL:
        print_pop(m_ostream, "HL");
        break;
    case LD_MC_A:
        print_ld(m_ostream, "(C)", "A");
        break;
    case PUSH_HL:
        print_push(m_ostream, "HL");
        break;
    case AND_n:
        print_and_n(m_ostream, get_next_byte());
        break;
    case RST_4:
        print_rst(m_ostream, 4);
        break;
    case ADD_SP_n:
        print_add_r_n(m_ostream, "SP", get_next_byte());
        break;
    case JP_MHL:
        print_jp_Mss(m_ostream, "HL");
        break;
    case LD_Mnn_A:
        print_ld_Mnn_dd(m_ostream, get_next_word(), "A");
        break;
    case XOR_n:
        print_xor_n(m_ostream, get_next_byte());
        break;
    case RST_5:
        print_rst(m_ostream, 5);
        break;
    case LDH_A_Mn:
        print_ldh_A_Mn(m_ostream, get_next_byte());
        break;
    case POP_AF:
        print_pop(m_ostream, "AF");
        break;
    case LD_A_MC:
        print_ld(m_ostream, "A", "(C)");
        break;
    case DI:
        print_di(m_ostream);
        break;
    case PUSH_AF:
        print_push(m_ostream, "AF");
        break;
    case OR_n:
        print_or_n(m_ostream, get_next_byte());
        break;
    case RST_6:
        print_rst(m_ostream, 6);
        break;
    case LD_HL_SPpn:
        print_ret(m_ostream, "M"); // TODO
        break;
    case LD_SP_HL:
        print_ld(m_ostream, "SP", "HL");
        break;
    case LD_A_Mnn:
        print_ld(m_ostream, "A", get_next_word());
        break;
    case EI:
        print_ei(m_ostream);
        break;
    case CP_n:
        print_cp(m_ostream, get_next_byte());
        break;
    case RST_7:
        print_rst(m_ostream, 7);
        break;
    default:
        m_ostream << "db " << hexify_wo_0x(m_opcode);
        break;
    }

    m_ostream << "\n";
}

void Disassembler::print_next_bits_instruction(u8 bits_opcode)
{
    switch (bits_opcode) {
    case RLC_B:
        print_rlc(m_ostream, "B");
        break;
    case RLC_C:
        print_rlc(m_ostream, "C");
        break;
    case RLC_D:
        print_rlc(m_ostream, "D");
        break;
    case RLC_E:
        print_rlc(m_ostream, "E");
        break;
    case RLC_H:
        print_rlc(m_ostream, "H");
        break;
    case RLC_L:
        print_rlc(m_ostream, "L");
        break;
    case RLC_MHL:
        print_rlc(m_ostream, "(HL)");
        break;
    case RLC_A:
        print_rlc(m_ostream, "A");
        break;
    case RRC_B:
        print_rrc(m_ostream, "B");
        break;
    case RRC_C:
        print_rrc(m_ostream, "C");
        break;
    case RRC_D:
        print_rrc(m_ostream, "D");
        break;
    case RRC_E:
        print_rrc(m_ostream, "E");
        break;
    case RRC_H:
        print_rrc(m_ostream, "H");
        break;
    case RRC_L:
        print_rrc(m_ostream, "L");
        break;
    case RRC_MHL:
        print_rrc(m_ostream, "(HL)");
        break;
    case RRC_A:
        print_rrc(m_ostream, "A");
        break;
    case RL_B:
        print_rl(m_ostream, "B");
        break;
    case RL_C:
        print_rl(m_ostream, "C");
        break;
    case RL_D:
        print_rl(m_ostream, "D");
        break;
    case RL_E:
        print_rl(m_ostream, "E");
        break;
    case RL_H:
        print_rl(m_ostream, "H");
        break;
    case RL_L:
        print_rl(m_ostream, "L");
        break;
    case RL_MHL:
        print_rl(m_ostream, "(HL)");
        break;
    case RL_A:
        print_rl(m_ostream, "A");
        break;
    case RR_B:
        print_rr(m_ostream, "B");
        break;
    case RR_C:
        print_rr(m_ostream, "C");
        break;
    case RR_D:
        print_rr(m_ostream, "D");
        break;
    case RR_E:
        print_rr(m_ostream, "E");
        break;
    case RR_H:
        print_rr(m_ostream, "H");
        break;
    case RR_L:
        print_rr(m_ostream, "L");
        break;
    case RR_MHL:
        print_rr(m_ostream, "(HL)");
        break;
    case RR_A:
        print_rr(m_ostream, "A");
        break;
    case SLA_B:
        print_sla(m_ostream, "B");
        break;
    case SLA_C:
        print_sla(m_ostream, "C");
        break;
    case SLA_D:
        print_sla(m_ostream, "D");
        break;
    case SLA_E:
        print_sla(m_ostream, "E");
        break;
    case SLA_H:
        print_sla(m_ostream, "H");
        break;
    case SLA_L:
        print_sla(m_ostream, "L");
        break;
    case SLA_MHL:
        print_sla(m_ostream, "(HL)");
        break;
    case SLA_A:
        print_sla(m_ostream, "A");
        break;
    case SRA_B:
        print_sra(m_ostream, "B");
        break;
    case SRA_C:
        print_sra(m_ostream, "C");
        break;
    case SRA_D:
        print_sra(m_ostream, "D");
        break;
    case SRA_E:
        print_sra(m_ostream, "E");
        break;
    case SRA_H:
        print_sra(m_ostream, "H");
        break;
    case SRA_L:
        print_sra(m_ostream, "L");
        break;
    case SRA_MHL:
        print_sra(m_ostream, "(HL)");
        break;
    case SRA_A:
        print_sra(m_ostream, "A");
        break;
    case SWAP_B:
        print_swap(m_ostream, "B");
        break;
    case SWAP_C:
        print_swap(m_ostream, "C");
        break;
    case SWAP_D:
        print_swap(m_ostream, "D");
        break;
    case SWAP_E:
        print_swap(m_ostream, "E");
        break;
    case SWAP_H:
        print_swap(m_ostream, "H");
        break;
    case SWAP_L:
        print_swap(m_ostream, "L");
        break;
    case SWAP_MHL:
        print_swap(m_ostream, "(HL)");
        break;
    case SWAP_A:
        print_swap(m_ostream, "A");
        break;
    case SRL_B:
        print_srl(m_ostream, "B");
        break;
    case SRL_C:
        print_srl(m_ostream, "C");
        break;
    case SRL_D:
        print_srl(m_ostream, "D");
        break;
    case SRL_E:
        print_srl(m_ostream, "E");
        break;
    case SRL_H:
        print_srl(m_ostream, "H");
        break;
    case SRL_L:
        print_srl(m_ostream, "L");
        break;
    case SRL_MHL:
        print_srl(m_ostream, "(HL)");
        break;
    case SRL_A:
        print_srl(m_ostream, "A");
        break;
    case BIT_0_B:
        print_bit(m_ostream, 0, "B");
        break;
    case BIT_0_C:
        print_bit(m_ostream, 0, "C");
        break;
    case BIT_0_D:
        print_bit(m_ostream, 0, "D");
        break;
    case BIT_0_E:
        print_bit(m_ostream, 0, "E");
        break;
    case BIT_0_H:
        print_bit(m_ostream, 0, "H");
        break;
    case BIT_0_L:
        print_bit(m_ostream, 0, "L");
        break;
    case BIT_0_MHL:
        print_bit(m_ostream, 0, "(HL)");
        break;
    case BIT_0_A:
        print_bit(m_ostream, 0, "A");
        break;
    case BIT_1_B:
        print_bit(m_ostream, 1, "B");
        break;
    case BIT_1_C:
        print_bit(m_ostream, 1, "C");
        break;
    case BIT_1_D:
        print_bit(m_ostream, 1, "D");
        break;
    case BIT_1_E:
        print_bit(m_ostream, 1, "E");
        break;
    case BIT_1_H:
        print_bit(m_ostream, 1, "H");
        break;
    case BIT_1_L:
        print_bit(m_ostream, 1, "L");
        break;
    case BIT_1_MHL:
        print_bit(m_ostream, 1, "(HL)");
        break;
    case BIT_1_A:
        print_bit(m_ostream, 1, "A");
        break;
    case BIT_2_B:
        print_bit(m_ostream, 2, "B");
        break;
    case BIT_2_C:
        print_bit(m_ostream, 2, "C");
        break;
    case BIT_2_D:
        print_bit(m_ostream, 2, "D");
        break;
    case BIT_2_E:
        print_bit(m_ostream, 2, "E");
        break;
    case BIT_2_H:
        print_bit(m_ostream, 2, "H");
        break;
    case BIT_2_L:
        print_bit(m_ostream, 2, "L");
        break;
    case BIT_2_MHL:
        print_bit(m_ostream, 2, "(HL)");
        break;
    case BIT_2_A:
        print_bit(m_ostream, 2, "A");
        break;
    case BIT_3_B:
        print_bit(m_ostream, 3, "B");
        break;
    case BIT_3_C:
        print_bit(m_ostream, 3, "C");
        break;
    case BIT_3_D:
        print_bit(m_ostream, 3, "D");
        break;
    case BIT_3_E:
        print_bit(m_ostream, 3, "E");
        break;
    case BIT_3_H:
        print_bit(m_ostream, 3, "H");
        break;
    case BIT_3_L:
        print_bit(m_ostream, 3, "L");
        break;
    case BIT_3_MHL:
        print_bit(m_ostream, 3, "(HL)");
        break;
    case BIT_3_A:
        print_bit(m_ostream, 3, "A");
        break;
    case BIT_4_B:
        print_bit(m_ostream, 4, "B");
        break;
    case BIT_4_C:
        print_bit(m_ostream, 4, "C");
        break;
    case BIT_4_D:
        print_bit(m_ostream, 4, "D");
        break;
    case BIT_4_E:
        print_bit(m_ostream, 4, "E");
        break;
    case BIT_4_H:
        print_bit(m_ostream, 4, "H");
        break;
    case BIT_4_L:
        print_bit(m_ostream, 4, "L");
        break;
    case BIT_4_MHL:
        print_bit(m_ostream, 4, "(HL)");
        break;
    case BIT_4_A:
        print_bit(m_ostream, 4, "A");
        break;
    case BIT_5_B:
        print_bit(m_ostream, 5, "B");
        break;
    case BIT_5_C:
        print_bit(m_ostream, 5, "C");
        break;
    case BIT_5_D:
        print_bit(m_ostream, 5, "D");
        break;
    case BIT_5_E:
        print_bit(m_ostream, 5, "E");
        break;
    case BIT_5_H:
        print_bit(m_ostream, 5, "H");
        break;
    case BIT_5_L:
        print_bit(m_ostream, 5, "L");
        break;
    case BIT_5_MHL:
        print_bit(m_ostream, 5, "(HL)");
        break;
    case BIT_5_A:
        print_bit(m_ostream, 5, "A");
        break;
    case BIT_6_B:
        print_bit(m_ostream, 6, "B");
        break;
    case BIT_6_C:
        print_bit(m_ostream, 6, "C");
        break;
    case BIT_6_D:
        print_bit(m_ostream, 6, "D");
        break;
    case BIT_6_E:
        print_bit(m_ostream, 6, "E");
        break;
    case BIT_6_H:
        print_bit(m_ostream, 6, "H");
        break;
    case BIT_6_L:
        print_bit(m_ostream, 6, "L");
        break;
    case BIT_6_MHL:
        print_bit(m_ostream, 6, "(HL)");
        break;
    case BIT_6_A:
        print_bit(m_ostream, 6, "A");
        break;
    case BIT_7_B:
        print_bit(m_ostream, 7, "B");
        break;
    case BIT_7_C:
        print_bit(m_ostream, 7, "C");
        break;
    case BIT_7_D:
        print_bit(m_ostream, 7, "D");
        break;
    case BIT_7_E:
        print_bit(m_ostream, 7, "E");
        break;
    case BIT_7_H:
        print_bit(m_ostream, 7, "H");
        break;
    case BIT_7_L:
        print_bit(m_ostream, 7, "L");
        break;
    case BIT_7_MHL:
        print_bit(m_ostream, 7, "(HL)");
        break;
    case BIT_7_A:
        print_bit(m_ostream, 7, "A");
        break;
    case RES_0_B:
        print_res(m_ostream, 0, "B");
        break;
    case RES_0_C:
        print_res(m_ostream, 0, "C");
        break;
    case RES_0_D:
        print_res(m_ostream, 0, "D");
        break;
    case RES_0_E:
        print_res(m_ostream, 0, "E");
        break;
    case RES_0_H:
        print_res(m_ostream, 0, "H");
        break;
    case RES_0_L:
        print_res(m_ostream, 0, "L");
        break;
    case RES_0_MHL:
        print_res(m_ostream, 0, "(HL)");
        break;
    case RES_0_A:
        print_res(m_ostream, 0, "A");
        break;
    case RES_1_B:
        print_res(m_ostream, 1, "B");
        break;
    case RES_1_C:
        print_res(m_ostream, 1, "C");
        break;
    case RES_1_D:
        print_res(m_ostream, 1, "D");
        break;
    case RES_1_E:
        print_res(m_ostream, 1, "E");
        break;
    case RES_1_H:
        print_res(m_ostream, 1, "H");
        break;
    case RES_1_L:
        print_res(m_ostream, 1, "L");
        break;
    case RES_1_MHL:
        print_res(m_ostream, 1, "(HL)");
        break;
    case RES_1_A:
        print_res(m_ostream, 1, "A");
        break;
    case RES_2_B:
        print_res(m_ostream, 2, "B");
        break;
    case RES_2_C:
        print_res(m_ostream, 2, "C");
        break;
    case RES_2_D:
        print_res(m_ostream, 2, "D");
        break;
    case RES_2_E:
        print_res(m_ostream, 2, "E");
        break;
    case RES_2_H:
        print_res(m_ostream, 2, "H");
        break;
    case RES_2_L:
        print_res(m_ostream, 2, "L");
        break;
    case RES_2_MHL:
        print_res(m_ostream, 2, "(HL)");
        break;
    case RES_2_A:
        print_res(m_ostream, 2, "A");
        break;
    case RES_3_B:
        print_res(m_ostream, 3, "B");
        break;
    case RES_3_C:
        print_res(m_ostream, 3, "C");
        break;
    case RES_3_D:
        print_res(m_ostream, 3, "D");
        break;
    case RES_3_E:
        print_res(m_ostream, 3, "E");
        break;
    case RES_3_H:
        print_res(m_ostream, 3, "H");
        break;
    case RES_3_L:
        print_res(m_ostream, 3, "L");
        break;
    case RES_3_MHL:
        print_res(m_ostream, 3, "(HL)");
        break;
    case RES_3_A:
        print_res(m_ostream, 3, "A");
        break;
    case RES_4_B:
        print_res(m_ostream, 4, "B");
        break;
    case RES_4_C:
        print_res(m_ostream, 4, "C");
        break;
    case RES_4_D:
        print_res(m_ostream, 4, "D");
        break;
    case RES_4_E:
        print_res(m_ostream, 4, "E");
        break;
    case RES_4_H:
        print_res(m_ostream, 4, "H");
        break;
    case RES_4_L:
        print_res(m_ostream, 4, "L");
        break;
    case RES_4_MHL:
        print_res(m_ostream, 4, "(HL)");
        break;
    case RES_4_A:
        print_res(m_ostream, 4, "A");
        break;
    case RES_5_B:
        print_res(m_ostream, 5, "B");
        break;
    case RES_5_C:
        print_res(m_ostream, 5, "C");
        break;
    case RES_5_D:
        print_res(m_ostream, 5, "D");
        break;
    case RES_5_E:
        print_res(m_ostream, 5, "E");
        break;
    case RES_5_H:
        print_res(m_ostream, 5, "H");
        break;
    case RES_5_L:
        print_res(m_ostream, 5, "L");
        break;
    case RES_5_MHL:
        print_res(m_ostream, 5, "(HL)");
        break;
    case RES_5_A:
        print_res(m_ostream, 5, "A");
        break;
    case RES_6_B:
        print_res(m_ostream, 6, "B");
        break;
    case RES_6_C:
        print_res(m_ostream, 6, "C");
        break;
    case RES_6_D:
        print_res(m_ostream, 6, "D");
        break;
    case RES_6_E:
        print_res(m_ostream, 6, "E");
        break;
    case RES_6_H:
        print_res(m_ostream, 6, "H");
        break;
    case RES_6_L:
        print_res(m_ostream, 6, "L");
        break;
    case RES_6_MHL:
        print_res(m_ostream, 6, "(HL)");
        break;
    case RES_6_A:
        print_res(m_ostream, 6, "A");
        break;
    case RES_7_B:
        print_res(m_ostream, 7, "B");
        break;
    case RES_7_C:
        print_res(m_ostream, 7, "C");
        break;
    case RES_7_D:
        print_res(m_ostream, 7, "D");
        break;
    case RES_7_E:
        print_res(m_ostream, 7, "E");
        break;
    case RES_7_H:
        print_res(m_ostream, 7, "H");
        break;
    case RES_7_L:
        print_res(m_ostream, 7, "L");
        break;
    case RES_7_MHL:
        print_res(m_ostream, 7, "(HL)");
        break;
    case RES_7_A:
        print_res(m_ostream, 7, "A");
        break;
    case SET_0_B:
        print_set(m_ostream, 0, "B");
        break;
    case SET_0_C:
        print_set(m_ostream, 0, "C");
        break;
    case SET_0_D:
        print_set(m_ostream, 0, "D");
        break;
    case SET_0_E:
        print_set(m_ostream, 0, "E");
        break;
    case SET_0_H:
        print_set(m_ostream, 0, "H");
        break;
    case SET_0_L:
        print_set(m_ostream, 0, "L");
        break;
    case SET_0_MHL:
        print_set(m_ostream, 0, "(HL)");
        break;
    case SET_0_A:
        print_set(m_ostream, 0, "A");
        break;
    case SET_1_B:
        print_set(m_ostream, 1, "B");
        break;
    case SET_1_C:
        print_set(m_ostream, 1, "C");
        break;
    case SET_1_D:
        print_set(m_ostream, 1, "D");
        break;
    case SET_1_E:
        print_set(m_ostream, 1, "E");
        break;
    case SET_1_H:
        print_set(m_ostream, 1, "H");
        break;
    case SET_1_L:
        print_set(m_ostream, 1, "L");
        break;
    case SET_1_MHL:
        print_set(m_ostream, 1, "(HL)");
        break;
    case SET_1_A:
        print_set(m_ostream, 1, "A");
        break;
    case SET_2_B:
        print_set(m_ostream, 2, "B");
        break;
    case SET_2_C:
        print_set(m_ostream, 2, "C");
        break;
    case SET_2_D:
        print_set(m_ostream, 2, "D");
        break;
    case SET_2_E:
        print_set(m_ostream, 2, "E");
        break;
    case SET_2_H:
        print_set(m_ostream, 2, "H");
        break;
    case SET_2_L:
        print_set(m_ostream, 2, "L");
        break;
    case SET_2_MHL:
        print_set(m_ostream, 2, "(HL)");
        break;
    case SET_2_A:
        print_set(m_ostream, 2, "A");
        break;
    case SET_3_B:
        print_set(m_ostream, 3, "B");
        break;
    case SET_3_C:
        print_set(m_ostream, 3, "C");
        break;
    case SET_3_D:
        print_set(m_ostream, 3, "D");
        break;
    case SET_3_E:
        print_set(m_ostream, 3, "E");
        break;
    case SET_3_H:
        print_set(m_ostream, 3, "H");
        break;
    case SET_3_L:
        print_set(m_ostream, 3, "L");
        break;
    case SET_3_MHL:
        print_set(m_ostream, 3, "(HL)");
        break;
    case SET_3_A:
        print_set(m_ostream, 3, "A");
        break;
    case SET_4_B:
        print_set(m_ostream, 4, "B");
        break;
    case SET_4_C:
        print_set(m_ostream, 4, "C");
        break;
    case SET_4_D:
        print_set(m_ostream, 4, "D");
        break;
    case SET_4_E:
        print_set(m_ostream, 4, "E");
        break;
    case SET_4_H:
        print_set(m_ostream, 4, "H");
        break;
    case SET_4_L:
        print_set(m_ostream, 4, "L");
        break;
    case SET_4_MHL:
        print_set(m_ostream, 4, "(HL)");
        break;
    case SET_4_A:
        print_set(m_ostream, 4, "A");
        break;
    case SET_5_B:
        print_set(m_ostream, 5, "B");
        break;
    case SET_5_C:
        print_set(m_ostream, 5, "C");
        break;
    case SET_5_D:
        print_set(m_ostream, 5, "D");
        break;
    case SET_5_E:
        print_set(m_ostream, 5, "E");
        break;
    case SET_5_H:
        print_set(m_ostream, 5, "H");
        break;
    case SET_5_L:
        print_set(m_ostream, 5, "L");
        break;
    case SET_5_MHL:
        print_set(m_ostream, 5, "(HL)");
        break;
    case SET_5_A:
        print_set(m_ostream, 5, "A");
        break;
    case SET_6_B:
        print_set(m_ostream, 6, "B");
        break;
    case SET_6_C:
        print_set(m_ostream, 6, "C");
        break;
    case SET_6_D:
        print_set(m_ostream, 6, "D");
        break;
    case SET_6_E:
        print_set(m_ostream, 6, "E");
        break;
    case SET_6_H:
        print_set(m_ostream, 6, "H");
        break;
    case SET_6_L:
        print_set(m_ostream, 6, "L");
        break;
    case SET_6_MHL:
        print_set(m_ostream, 6, "(HL)");
        break;
    case SET_6_A:
        print_set(m_ostream, 6, "A");
        break;
    case SET_7_B:
        print_set(m_ostream, 7, "B");
        break;
    case SET_7_C:
        print_set(m_ostream, 7, "C");
        break;
    case SET_7_D:
        print_set(m_ostream, 7, "D");
        break;
    case SET_7_E:
        print_set(m_ostream, 7, "E");
        break;
    case SET_7_H:
        print_set(m_ostream, 7, "H");
        break;
    case SET_7_L:
        print_set(m_ostream, 7, "L");
        break;
    case SET_7_MHL:
        print_set(m_ostream, 7, "(HL)");
        break;
    case SET_7_A:
        print_set(m_ostream, 7, "A");
        break;
    default:
        m_ostream << "db " << hexify_wo_0x(bits_opcode);
        break;
    }
}

void Disassembler::print_cartridge_header()
{
    if (0x0104 <= m_pc && m_pc <= 0x0133 + 1) { // Nintendo logo
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Nintendo logo)\n";
    } else if (0x0134 <= m_pc && m_pc <= 0x0142 + 1) { // Title
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Title)\n";
    } else if (0x0143 + 1 == m_pc) { // Color / not color
        m_ostream << "db " << hexify_wo_0x(m_opcode) << (m_opcode == 0x80 ? " (color GB)\n" : " (not color GB)\n");
    } else if (0x0144 <= m_pc && m_pc <= 0x0145 + 1) { // New licensee code
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (New licensee code)\n";
    } else if (0x0146 + 1 == m_pc) { // GB / SGB indicator
        m_ostream << "db " << hexify_wo_0x(m_opcode) << (m_opcode == 0x03 ? " (SGB)\n" : " (GB)\n");
    } else if (0x0147 + 1 == m_pc) { // Cartridge type
        switch (m_opcode) {
        case 0x00:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: ROM ONLY)\n";
            break;
        case 0x01:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC1)\n";
            break;
        case 0x02:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC1+RAM)\n";
            break;
        case 0x03:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC1+RAM+BATTERY)\n";
            break;
        case 0x05:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC2)\n";
            break;
        case 0x06:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC2+BATTERY)\n";
            break;
        case 0x08:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: ROM+RAM)\n";
            break;
        case 0x09:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: ROM+RAM+BATTER)\n";
            break;
        case 0x0b:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MMM01)\n";
            break;
        case 0x0c:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MMM01+RAM)\n";
            break;
        case 0x0d:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MMM01+RAM+BATTERY)\n";
            break;
        case 0x0f:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC3+TIMER+BATTERY)\n";
            break;
        case 0x10:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC3+TIMER+RAM+BATTERY)\n";
            break;
        case 0x11:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC3)\n";
            break;
        case 0x12:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC3+RAM)\n";
            break;
        case 0x13:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC3+RAM+BATTERY)\n";
            break;
        case 0x19:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5)\n";
            break;
        case 0x1a:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5+RAM)\n";
            break;
        case 0x1b:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5+RAM+BATTERY)\n";
            break;
        case 0x1c:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5+RUMBLE)\n";
            break;
        case 0x1d:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5+RUMBLE+RAM)\n";
            break;
        case 0x1e:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC5+RUMBLE+RAM+BATTERY)\n";
            break;
        case 0x20:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC6)\n";
            break;
        case 0x22:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: MBC7+SENSOR+RUMBLE+RAM+BATTERY)\n";
            break;
        case 0xfc:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: POCKET CAMERA)\n";
            break;
        case 0xfd:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: BANDAI TAMA5)\n";
            break;
        case 0xfe:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: HuC3)\n";
            break;
        case 0xff:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: HuC1+RAM+BATTERY)\n";
            break;
        default:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Cartridge type: Unknown)\n";
            break;
        }
    } else if (0x0148 + 1 == m_pc) { // ROM size
        switch (m_opcode) {
        case 0x00:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 32KByte (no ROM banking))\n";
            break;
        case 0x01:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 64KByte (4 banks))\n";
            break;
        case 0x02:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 128KByte (8 banks))\n";
            break;
        case 0x03:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 256KByte (16 banks))\n";
            break;
        case 0x04:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 512KByte (32 banks))\n";
            break;
        case 0x05:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 1MByte (64 banks)  - only 63 banks used by MBC1)\n";
            break;
        case 0x06:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 2MByte (128 banks) - only 125 banks used by MBC1)\n";
            break;
        case 0x07:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 4MByte (256 banks))\n";
            break;
        case 0x08:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 8MByte (512 banks))\n";
            break;
        case 0x52:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 1.1MByte (72 banks))\n";
            break;
        case 0x53:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 1.2MByte (80 banks))\n";
            break;
        case 0x54:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: 1.5MByte (96 banks))\n";
            break;
        default:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (ROM size: Unknown)\n";
            break;
        }
    } else if (0x0149 + 1 == m_pc) { // RAM size
        switch (m_opcode) {
        case 0x00:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: None)\n";
            break;
        case 0x01:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: 2 KBytes)\n";
            break;
        case 0x02:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: 8 KBytes)\n";
            break;
        case 0x03:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: 32 KBytes (4 banks of 8KBytes each))\n";
            break;
        case 0x04:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: 128 KBytes (16 banks of 8KBytes each))\n";
            break;
        case 0x05:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: 64 KBytes (8 banks of 8KBytes each))\n";
            break;
        default:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (RAM size: Unknown)\n";
            break;
        }
    } else if (0x014a + 1 == m_pc) { // Destination code
        switch (m_opcode) {
        case 0x00:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Destination code: Japanese)\n";
            break;
        case 0x01:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Destination code: Non-Japanese)\n";
            break;
        default:
            m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Destination code: Unknown)\n";
            break;
        }
    } else if (0x014b + 1 == m_pc) { // Old licensee code
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Old licensee code)\n";
    } else if (0x014c + 1 == m_pc) { // Mask ROM version number
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Mask ROM version number)\n";
    } else if (0x014d + 1 == m_pc) { // Header checksum
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Header checksum)\n";
    } else if (0x014e <= m_pc && m_pc <= 0x014f + 1) { // Global checksum
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Global checksum)\n";
    } else {
        m_ostream << "db " << hexify_wo_0x(m_opcode) << " (Description not implemented yet)\n";
    }
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
