#include <iostream>
#include "disassemblerZ80.h"
#include "instructions/instructions.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::exceptions::UnrecognizedOpcodeException;
    using emu::util::string::hexify;
    using emu::util::string::hexify_wo_0x;

    DisassemblerZ80::DisassemblerZ80(EmulatorMemory &memory, std::ostream &ostream)
            : m_memory(memory),
              m_memory_size(memory.size()),
              m_pc(0),
              m_opcode(0),
              m_ostream(ostream) {
    }

    void DisassemblerZ80::disassemble() {
        while (m_pc < m_memory_size) {
            print_next_instruction();
        }
    }

    void DisassemblerZ80::print_next_instruction() {
        m_ostream << hexify_wo_0x(m_pc, 4) << "\t\t";

        m_opcode = get_next_byte().farg;

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
            case EX_AF_AFP:
                print_ex(m_ostream, "AF", "AF'");
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
            case DJNZ:
                print_djnz(m_ostream, get_next_byte());
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
            case LD_Mnn_HL:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "HL");
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
            case LD_HL_Mnn:
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
            case LD_Mnn_A:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "A");
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
            case LD_A_Mnn:
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
            case OUT:
                print_out(m_ostream, get_next_byte());
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
            case EXX:
                print_exx(m_ostream);
                break;
            case JP_C:
                print_jp_cc_nn(m_ostream, get_next_word(), "C");
                break;
            case IN:
                print_in(m_ostream, get_next_byte());
                break;
            case CALL_C:
                print_call_cc_nn(m_ostream, get_next_word(), "C");
                break;
            case IX:
                print_next_ixy_instruction(get_next_byte().farg, "IX");
                break;
            case SBC_A_n:
                print_sbc_r_n(m_ostream, "A", get_next_byte());
                break;
            case RST_3:
                print_rst(m_ostream, 3);
                break;
            case RET_PO:
                print_ret(m_ostream, "PO");
                break;
            case POP_HL:
                print_pop(m_ostream, "HL");
                break;
            case JP_PO:
                print_jp_cc_nn(m_ostream, get_next_word(), "PO");
                break;
            case EX_MSP_HL:
                print_ex(m_ostream, "(SP)", "HL");
                break;
            case CALL_PO:
                print_call_cc_nn(m_ostream, get_next_word(), "PO");
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
            case RET_PE:
                print_ret(m_ostream, "PE");
                break;
            case JP_MHL:
                print_jp_Mss(m_ostream, "HL");
                break;
            case JP_PE:
                print_jp_cc_nn(m_ostream, get_next_word(), "PE");
                break;
            case EX_DE_HL:
                print_ex(m_ostream, "DE", "HL");
                break;
            case CALL_PE:
                print_call_cc_nn(m_ostream, get_next_word(), "PE");
                break;
            case EXTD:
                print_next_extd_instruction(get_next_byte().farg);
                break;
            case XOR_n:
                print_xor_n(m_ostream, get_next_byte());
                break;
            case RST_5:
                print_rst(m_ostream, 5);
                break;
            case RET_P:
                print_ret(m_ostream, "P");
                break;
            case POP_AF:
                print_pop(m_ostream, "AF");
                break;
            case JP_P:
                print_jp_cc_nn(m_ostream, get_next_word(), "P");
                break;
            case DI:
                print_di(m_ostream);
                break;
            case CALL_P:
                print_call_cc_nn(m_ostream, get_next_word(), "P");
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
            case RET_M:
                print_ret(m_ostream, "M");
                break;
            case LD_SP_HL:
                print_ld(m_ostream, "SP", "HL");
                break;
            case JP_M:
                print_jp_cc_nn(m_ostream, get_next_word(), "M");
                break;
            case EI:
                print_ei(m_ostream);
                break;
            case CALL_M:
                print_call_cc_nn(m_ostream, get_next_word(), "M");
                break;
            case IY:
                print_next_ixy_instruction(get_next_byte().farg, "IY");
                break;
            case CP_n:
                print_cp(m_ostream, get_next_byte());
                break;
            case RST_7:
                print_rst(m_ostream, 7);
                break;
            default:
                throw UnrecognizedOpcodeException(m_opcode);
        }

        m_ostream << "\n";
    }

    void DisassemblerZ80::print_next_bits_instruction(u8 bits_opcode) {
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
            case SLL_B:
                print_sll(m_ostream, "B");
                break;
            case SLL_C:
                print_sll(m_ostream, "C");
                break;
            case SLL_D:
                print_sll(m_ostream, "D");
                break;
            case SLL_E:
                print_sll(m_ostream, "E");
                break;
            case SLL_H:
                print_sll(m_ostream, "H");
                break;
            case SLL_L:
                print_sll(m_ostream, "L");
                break;
            case SLL_MHL:
                print_sll(m_ostream, "(HL)");
                break;
            case SLL_A:
                print_sll(m_ostream, "A");
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
                m_ostream << hexify(bits_opcode) << " (data)";
                break;
        }
    }

    void DisassemblerZ80::print_next_ixy_instruction(u8 ixy_opcode, const std::string &ixy_reg) {
        switch (ixy_opcode) {
            case INC_B_UNDOC:
                print_inc_undocumented(m_ostream, "B");
                break;
            case DEC_B_UNDOC:
                print_dec_undocumented(m_ostream, "B");
                break;
            case LD_B_n_UNDOC:
                print_ld_undocumented(m_ostream, "B", get_next_byte());
                break;
            case ADD_IXY_BC:
                print_add(m_ostream, ixy_reg, "BC");
                break;
            case INC_C_UNDOC:
                print_inc_undocumented(m_ostream, "C");
                break;
            case DEC_C_UNDOC:
                print_dec_undocumented(m_ostream, "C");
                break;
            case LD_C_n_UNDOC:
                print_ld_undocumented(m_ostream, "C", get_next_byte());
                break;
            case INC_D_UNDOC:
                print_inc_undocumented(m_ostream, "D");
                break;
            case DEC_D_UNDOC:
                print_dec_undocumented(m_ostream, "D");
                break;
            case LD_D_n_UNDOC:
                print_ld_undocumented(m_ostream, "D", get_next_byte());
                break;
            case ADD_IXY_DE:
                print_add(m_ostream, ixy_reg, "DE");
                break;
            case INC_E_UNDOC:
                print_inc_undocumented(m_ostream, "E");
                break;
            case DEC_E_UNDOC:
                print_dec_undocumented(m_ostream, "E");
                break;
            case LD_E_n_UNDOC:
                print_ld_undocumented(m_ostream, "E", get_next_byte());
                break;
            case LD_IXY_nn:
                print_ld_dd_nn(m_ostream, ixy_reg, get_next_word());
                break;
            case LD_Mnn_IXY:
                print_ld_Mnn_dd(m_ostream, get_next_word(), ixy_reg);
                break;
            case INC_IXY:
                print_inc(m_ostream, ixy_reg);
                break;
            case INC_IXH_UNDOC:
                print_inc_undocumented(m_ostream, ixy_reg + "H");
                break;
            case DEC_IXH_UNDOC:
                print_dec_undocumented(m_ostream, ixy_reg + "H");
                break;
            case LD_IXYH_n_UNDOC:
                print_ld_undocumented(m_ostream, ixy_reg + "H", get_next_byte());
                break;
            case ADD_IXY_IXY:
                print_add(m_ostream, ixy_reg, ixy_reg);
                break;
            case LD_IXY_Mnn:
                print_ld(m_ostream, ixy_reg, get_next_word());
                break;
            case DEC_IXY:
                print_dec(m_ostream, ixy_reg);
                break;
            case INC_IXL_UNDOC:
                print_inc_undocumented(m_ostream, ixy_reg + "L");
                break;
            case DEC_IXL_UNDOC:
                print_dec_undocumented(m_ostream, ixy_reg + "L");
                break;
            case LD_IXYL_n_UNDOC:
                print_ld_undocumented(m_ostream, ixy_reg + "L", get_next_byte());
                break;
            case INC_MIXY_P_n:
                print_inc_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case DEC_MIXY_P_n:
                print_dec_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case LD_MIXY_P_n_d:
                print_ld_MixyPd_n(m_ostream, ixy_reg, get_next_word());
                break;
            case ADD_IXY_SP:
                print_add(m_ostream, ixy_reg, "SP");
                break;
            case INC_A_UNDOC:
                print_inc_undocumented(m_ostream, "A");
                break;
            case DEC_A_UNDOC:
                print_dec_undocumented(m_ostream, "A");
                break;
            case LD_A_n_UNDOC:
                print_ld_undocumented(m_ostream, "A", get_next_byte());
                break;
            case LD_B_B_UNDOC:
                print_ld_undocumented(m_ostream, "B", "B");
                break;
            case LD_B_C_UNDOC:
                print_ld_undocumented(m_ostream, "B", "C");
                break;
            case LD_B_D_UNDOC:
                print_ld_undocumented(m_ostream, "B", "D");
                break;
            case LD_B_E_UNDOC:
                print_ld_undocumented(m_ostream, "B", "E");
                break;
            case LD_B_IXYH_UNDOC:
                print_ld_undocumented(m_ostream, "B", ixy_reg + "H");
                break;
            case LD_B_IXYL_UNDOC:
                print_ld_undocumented(m_ostream, "B", ixy_reg + "L");
                break;
            case LD_B_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "B", ixy_reg, get_next_byte());
                break;
            case LD_B_A_UNDOC:
                print_ld_undocumented(m_ostream, "B", "A");
                break;
            case LD_C_B_UNDOC:
                print_ld_undocumented(m_ostream, "C", "B");
                break;
            case LD_C_C_UNDOC:
                print_ld_undocumented(m_ostream, "C", "C");
                break;
            case LD_C_D_UNDOC:
                print_ld_undocumented(m_ostream, "C", "D");
                break;
            case LD_C_E_UNDOC:
                print_ld_undocumented(m_ostream, "C", "E");
                break;
            case LD_C_IXYH_UNDOC:
                print_ld_undocumented(m_ostream, "C", ixy_reg + "H");
                break;
            case LD_C_IXYL_UNDOC:
                print_ld_undocumented(m_ostream, "C", ixy_reg + "L");
                break;
            case LD_C_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "C", ixy_reg, get_next_byte());
                break;
            case LD_C_A_UNDOC:
                print_ld_undocumented(m_ostream, "C", "A");
                break;
            case LD_D_B_UNDOC:
                print_ld_undocumented(m_ostream, "D", "B");
                break;
            case LD_D_C_UNDOC:
                print_ld_undocumented(m_ostream, "D", "C");
                break;
            case LD_D_D_UNDOC:
                print_ld_undocumented(m_ostream, "D", "D");
                break;
            case LD_D_E_UNDOC:
                print_ld_undocumented(m_ostream, "D", "E");
                break;
            case LD_D_IXYH_UNDOC:
                print_ld_undocumented(m_ostream, "D", ixy_reg + "H");
                break;
            case LD_D_IXYL_UNDOC:
                print_ld_undocumented(m_ostream, "D", ixy_reg + "L");
                break;
            case LD_D_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "D", ixy_reg, get_next_byte());
                break;
            case LD_D_A_UNDOC:
                print_ld_undocumented(m_ostream, "D", "A");
                break;
            case LD_E_B_UNDOC:
                print_ld_undocumented(m_ostream, "E", "B");
                break;
            case LD_E_C_UNDOC:
                print_ld_undocumented(m_ostream, "E", "C");
                break;
            case LD_E_D_UNDOC:
                print_ld_undocumented(m_ostream, "E", "D");
                break;
            case LD_E_E_UNDOC:
                print_ld_undocumented(m_ostream, "E", "E");
                break;
            case LD_E_IXYH_UNDOC:
                print_ld_undocumented(m_ostream, "E", ixy_reg + "H");
                break;
            case LD_E_IXYL_UNDOC:
                print_ld_undocumented(m_ostream, "E", ixy_reg + "L");
                break;
            case LD_E_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "E", ixy_reg, get_next_byte());
                break;
            case LD_E_A_UNDOC:
                print_ld_undocumented(m_ostream, "E", "A");
                break;
            case LD_H_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "H", ixy_reg, get_next_byte());
                break;
            case LD_L_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "L", ixy_reg, get_next_byte());
                break;
            case LD_IXYL_A_UNDOC:
                print_ld_undocumented(m_ostream, ixy_reg + "L", "A");
                break;
            case LD_MIXY_P_n_B:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "B");
                break;
            case LD_MIXY_P_n_C:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "C");
                break;
            case LD_MIXY_P_n_D:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "D");
                break;
            case LD_MIXY_P_n_E:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "E");
                break;
            case LD_MIXY_P_n_H:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "H");
                break;
            case LD_MIXY_P_n_L:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "L");
                break;
            case LD_MIXY_P_n_A:
                print_ld_MixyPd_r(m_ostream, ixy_reg, get_next_byte(), "A");
                break;
            case LD_A_B_UNDOC:
                print_ld_undocumented(m_ostream, "A", "B");
                break;
            case LD_A_C_UNDOC:
                print_ld_undocumented(m_ostream, "A", "C");
                break;
            case LD_A_D_UNDOC:
                print_ld_undocumented(m_ostream, "A", "D");
                break;
            case LD_A_E_UNDOC:
                print_ld_undocumented(m_ostream, "A", "E");
                break;
            case LD_A_IXYH_UNDOC:
                print_ld_undocumented(m_ostream, "A", ixy_reg + "H");
                break;
            case LD_A_IXYL_UNDOC:
                print_ld_undocumented(m_ostream, "A", ixy_reg + "L");
                break;
            case LD_A_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case LD_A_A_UNDOC:
                print_ld_undocumented(m_ostream, "A", "A");
                break;
            case ADD_A_B_UNDOC:
                print_add_undocumented(m_ostream, "A", "B");
                break;
            case ADD_A_C_UNDOC:
                print_add_undocumented(m_ostream, "A", "C");
                break;
            case ADD_A_D_UNDOC:
                print_add_undocumented(m_ostream, "A", "D");
                break;
            case ADD_A_E_UNDOC:
                print_add_undocumented(m_ostream, "A", "E");
                break;
            case ADD_A_IXYH_UNDOC:
                print_add(m_ostream, "A", ixy_reg + "H");
                break;
            case ADD_A_IXYL_UNDOC:
                print_add(m_ostream, "A", ixy_reg + "L");
                break;
            case ADD_A_MIXY_P_n:
                print_add_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case ADD_A_A_UNDOC:
                print_add_undocumented(m_ostream, "A", "A");
                break;
            case ADC_A_B_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", "B");
                break;
            case ADC_A_C_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", "C");
                break;
            case ADC_A_D_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", "D");
                break;
            case ADC_A_E_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", "E");
                break;
            case ADC_A_IXYH_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", ixy_reg + "H");
                break;
            case ADC_A_IXYL_UNDOC:
                print_adc_r_s(m_ostream, "A", ixy_reg + "L");
                break;
            case ADC_A_MIXY_P_n:
                print_adc_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case ADC_A_A_UNDOC:
                print_adc_r_s_undocumented(m_ostream, "A", "A");
                break;
            case SUB_B_UNDOC:
                print_sub_undocumented(m_ostream, "B");
                break;
            case SUB_C_UNDOC:
                print_sub_undocumented(m_ostream, "C");
                break;
            case SUB_D_UNDOC:
                print_sub_undocumented(m_ostream, "D");
                break;
            case SUB_E_UNDOC:
                print_sub_undocumented(m_ostream, "E");
                break;
            case SUB_IXYH_UNDOC:
                print_sub(m_ostream, ixy_reg + "H");
                break;
            case SUB_IXYL_UNDOC:
                print_sub(m_ostream, ixy_reg + "L");
                break;
            case SUB_MIXY_P_n:
                print_sub_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case SUB_A_UNDOC:
                print_sub_undocumented(m_ostream, "E");
                break;
            case SBC_A_B_UNDOC:
                print_sbc_r_s_undocumented(m_ostream, "A", "B");
                break;
            case SBC_A_C_UNDOC:
                print_sbc_r_s_undocumented(m_ostream, "A", "C");
                break;
            case SBC_A_D_UNDOC:
                print_sbc_r_s_undocumented(m_ostream, "A", "D");
                break;
            case SBC_A_E_UNDOC:
                print_sbc_r_s_undocumented(m_ostream, "A", "E");
                break;
            case SBC_A_IXYH_UNDOC:
                print_sbc_r_s(m_ostream, "A", ixy_reg + "H");
                break;
            case SBC_A_IXYL_UNDOC:
                print_sbc_r_s(m_ostream, "A", ixy_reg + "L");
                break;
            case SBC_A_MIXY_P_n:
                print_sbc_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case SBC_A_A_UNDOC:
                print_sbc_r_s_undocumented(m_ostream, "A", "A");
                break;
            case AND_B_UNDOC:
                print_and_r_undocumented(m_ostream, "B");
                break;
            case AND_C_UNDOC:
                print_and_r_undocumented(m_ostream, "C");
                break;
            case AND_D_UNDOC:
                print_and_r_undocumented(m_ostream, "D");
                break;
            case AND_E_UNDOC:
                print_and_r_undocumented(m_ostream, "E");
                break;
            case AND_IXYH_UNDOC:
                print_and_r(m_ostream, ixy_reg + "H");
                break;
            case AND_IXYL_UNDOC:
                print_and_r(m_ostream, ixy_reg + "L");
                break;
            case AND_MIXY_P_n:
                print_and_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case AND_A_UNDOC:
                print_and_r_undocumented(m_ostream, "A");
                break;
            case XOR_B_UNDOC:
                print_xor_r_undocumented(m_ostream, "B");
                break;
            case XOR_C_UNDOC:
                print_xor_r_undocumented(m_ostream, "C");
                break;
            case XOR_D_UNDOC:
                print_xor_r_undocumented(m_ostream, "D");
                break;
            case XOR_E_UNDOC:
                print_xor_r_undocumented(m_ostream, "E");
                break;
            case XOR_IXYH_UNDOC:
                print_xor_r(m_ostream, ixy_reg + "H");
                break;
            case XOR_IXYL_UNDOC:
                print_xor_r(m_ostream, ixy_reg + "L");
                break;
            case XOR_MIXY_P_n:
                print_xor_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case XOR_A_UNDOC:
                print_xor_r_undocumented(m_ostream, "A");
                break;
            case OR_B_UNDOC:
                print_or_r_undocumented(m_ostream, "B");
                break;
            case OR_C_UNDOC:
                print_or_r_undocumented(m_ostream, "C");
                break;
            case OR_D_UNDOC:
                print_or_r_undocumented(m_ostream, "D");
                break;
            case OR_E_UNDOC:
                print_or_r_undocumented(m_ostream, "E");
                break;
            case OR_IXYH_UNDOC:
                print_or_r(m_ostream, ixy_reg + "H");
                break;
            case OR_IXYL_UNDOC:
                print_or_r(m_ostream, ixy_reg + "L");
                break;
            case OR_MIXY_P_n:
                print_or_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case OR_A_UNDOC:
                print_or_r_undocumented(m_ostream, "A");
                break;
            case CP_B_UNDOC:
                print_cp_undocumented(m_ostream, "B");
                break;
            case CP_C_UNDOC:
                print_cp_undocumented(m_ostream, "C");
                break;
            case CP_D_UNDOC:
                print_cp_undocumented(m_ostream, "D");
                break;
            case CP_E_UNDOC:
                print_cp_undocumented(m_ostream, "E");
                break;
            case CP_IXYH_UNDOC:
                print_cp(m_ostream, ixy_reg + "H");
                break;
            case CP_IXYL_UNDOC:
                print_cp(m_ostream, ixy_reg + "L");
                break;
            case CP_MIXY_P_n:
                print_cp_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case CP_A_UNDOC:
                print_cp_undocumented(m_ostream, "A");
                break;
            case IXY_BITS:
                print_next_ixy_bits_instruction(get_next_word(), ixy_reg);
                break;
            case POP_IXY:
                print_pop(m_ostream, ixy_reg);
                break;
            case EX_MSP_IX:
                print_ex(m_ostream, "(SP)", ixy_reg);
                break;
            case PUSH_IXY:
                print_push(m_ostream, ixy_reg);
                break;
            case JP_MIXY:
                print_jp_Mss(m_ostream, ixy_reg);
                break;
            case LD_SP_IXY:
                print_ld(m_ostream, "SP", ixy_reg);
                break;
            default:
                m_ostream << hexify(ixy_opcode) << " (data)";
                break;
        }
    }

    void DisassemblerZ80::print_next_ixy_bits_instruction(NextWord args, const std::string &ixy_reg) {
        u8 d = args.farg;
        u8 ixy_bits_opcode = args.sarg;

        switch (ixy_bits_opcode) {
            case RLC_MIXY_P_n_B_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "B");
                break;
            case RLC_MIXY_P_n_C_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "C");
                break;
            case RLC_MIXY_P_n_D_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "D");
                break;
            case RLC_MIXY_P_n_E_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "E");
                break;
            case RLC_MIXY_P_n_H_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "H");
                break;
            case RLC_MIXY_P_n_L_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "L");
                break;
//            case RL_MIXY_P_n_B_UNDOC1:
//                break;
            case RLC_MIXY_P_n:
                print_rlc_MixyPn(m_ostream, ixy_reg, d);
                break;
            case RLC_MIXY_P_n_A_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, d, "A");
                break;
            case RRC_MIXY_P_n:
                print_rrc_MixyPn(m_ostream, ixy_reg, d);
                break;
            case RL_MIXY_P_n_B_UNDOC1:
                print_rl_MixyPn_r(m_ostream, ixy_reg, d, "B");
                break;
            case RL_MIXY_P_n:
                print_rl_MixyPn(m_ostream, ixy_reg, d);
                break;
            case RR_MIXY_P_n:
                print_rr_MixyPn(m_ostream, ixy_reg, d);
                break;
            case SLA_MIXY_P_n:
                print_sla_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case SRA_MIXY_P_n:
                print_sra_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case SLL_MIXY_P_n:
                print_sll_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case SRL_MIXY_P_n:
                print_srl_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case BIT_0_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case BIT_1_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 1, ixy_reg, d);
                break;
            case BIT_2_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 2, ixy_reg, d);
                break;
            case BIT_3_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 3, ixy_reg, d);
                break;
            case BIT_4_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 4, ixy_reg, d);
                break;
            case BIT_5_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 5, ixy_reg, d);
                break;
            case BIT_6_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 6, ixy_reg, d);
                break;
            case BIT_7_MIXY_P_n:
                print_bit_MixyPn(m_ostream, 7, ixy_reg, d);
                break;
            case RES_0_MIXY_P_n:
                print_res_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case RES_1_MIXY_P_n:
                print_res_MixyPn(m_ostream, 1, ixy_reg, d);
                break;
            case RES_2_MIXY_P_n:
                print_res_MixyPn(m_ostream, 2, ixy_reg, d);
                break;
            case RES_3_MIXY_P_n:
                print_res_MixyPn(m_ostream, 3, ixy_reg, d);
                break;
            case RES_4_MIXY_P_n:
                print_res_MixyPn(m_ostream, 4, ixy_reg, d);
                break;
            case RES_5_MIXY_P_n:
                print_res_MixyPn(m_ostream, 5, ixy_reg, d);
                break;
            case RES_6_MIXY_P_n:
                print_res_MixyPn(m_ostream, 6, ixy_reg, d);
                break;
            case RES_7_MIXY_P_n:
                print_res_MixyPn(m_ostream, 7, ixy_reg, d);
                break;
            case SET_0_MIXY_P_n:
                print_set_MixyPn(m_ostream, 0, ixy_reg, d);
                break;
            case SET_1_MIXY_P_n:
                print_set_MixyPn(m_ostream, 1, ixy_reg, d);
                break;
            case SET_2_MIXY_P_n:
                print_set_MixyPn(m_ostream, 2, ixy_reg, d);
                break;
            case SET_3_MIXY_P_n:
                print_set_MixyPn(m_ostream, 3, ixy_reg, d);
                break;
            case SET_4_MIXY_P_n:
                print_set_MixyPn(m_ostream, 4, ixy_reg, d);
                break;
            case SET_5_MIXY_P_n:
                print_set_MixyPn(m_ostream, 5, ixy_reg, d);
                break;
            case SET_6_MIXY_P_n:
                print_set_MixyPn(m_ostream, 6, ixy_reg, d);
                break;
            case SET_7_MIXY_P_n:
                print_set_MixyPn(m_ostream, 7, ixy_reg, d);
                break;
            default:
                throw UnrecognizedOpcodeException(ixy_bits_opcode, "IX/IY bits instructions");
        }
    }

    void DisassemblerZ80::print_next_extd_instruction(u8 extd_opcode) {
        switch (extd_opcode) {
            case IN_B_MC:
                print_in_r_Mr(m_ostream, "B", "C");
                break;
            case OUT_MC_B:
                print_out_Mr_r(m_ostream, "C", "B");
                break;
            case SBC_HL_BC:
                print_sbc_r_s(m_ostream, "HL", "BC");
                break;
            case LD_Mnn_BC:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "BC");
                break;
            case NEG_UNDOC1:
            case NEG_UNDOC2:
            case NEG_UNDOC3:
            case NEG_UNDOC4:
            case NEG_UNDOC5:
            case NEG_UNDOC6:
            case NEG:
                print_neg(m_ostream);
                break;
            case RETN:
                print_retn(m_ostream);
                break;
            case LD_I_A:
                print_ld(m_ostream, "I", "A");
                break;
            case IM_0_1:
            case IM_0_2:
                print_im(m_ostream, 0);
                break;
            case IN_C_MC:
                print_in_r_Mr(m_ostream, "C", "C");
                break;
            case OUT_MC_C:
                print_out_Mr_r(m_ostream, "C", "C");
                break;
            case ADC_HL_BC:
                print_adc_r_s(m_ostream, "HL", "BC");
                break;
            case LD_BC_Mnn:
                print_ld(m_ostream, "BC", get_next_word());
                break;
            case RETI:
                print_reti(m_ostream);
                break;
            case LD_R_A:
                print_ld(m_ostream, "R", "A");
                break;
            case IN_D_MC:
                print_in_r_Mr(m_ostream, "D", "C");
                break;
            case OUT_MC_D:
                print_out_Mr_r(m_ostream, "C", "D");
                break;
            case SBC_HL_DE:
                print_sbc_r_s(m_ostream, "HL", "DE");
                break;
            case LD_Mnn_DE:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "DE");
                break;
            case IM_2:
                print_im(m_ostream, 2);
                break;
            case LD_A_R:
                print_ld(m_ostream, "A", "R");
                break;
            case IN_H_MC:
                print_in_r_Mr(m_ostream, "H", "C");
                break;
            case OUT_MC_H:
                print_out_Mr_r(m_ostream, "C", "H");
                break;
            case SBC_HL_HL:
                print_sbc_r_s(m_ostream, "HL", "HL");
                break;
            case LD_Mnn_HL_UNDOC:
                print_ld_Mnn_dd_undocumented(m_ostream, get_next_word(), "HL");
                break;
            case SBC_HL_SP:
                print_sbc_r_s(m_ostream, "HL", "SP");
                break;
            case IM_1_1:
            case IM_1_2:
                print_im(m_ostream, 1);
                break;
            case LD_A_I:
                print_ld(m_ostream, "A", "I");
                break;
            case IN_A_MC:
                print_in_r_Mr(m_ostream, "A", "C");
                break;
            case OUT_MC_A:
                print_out_Mr_r(m_ostream, "C", "A");
                break;
            case IN_E_MC:
                print_in_r_Mr(m_ostream, "E", "C");
                break;
            case OUT_MC_E:
                print_out_Mr_r(m_ostream, "C", "E");
                break;
            case ADC_HL_DE:
                print_adc_r_s(m_ostream, "HL", "DE");
                break;
            case LD_DE_Mnn:
                print_ld_dd_Mnn(m_ostream, "DE", get_next_word());
                break;
            case RRD:
                print_rrd(m_ostream);
                break;
            case IN_L_MC:
                print_in_r_Mr(m_ostream, "L", "C");
                break;
            case OUT_MC_L:
                print_out_Mr_r(m_ostream, "C", "L");
                break;
            case ADC_HL_HL:
                print_adc_r_s(m_ostream, "HL", "HL");
                break;
            case LD_HL_Mnn_UNDOC:
                print_ld_dd_Mnn_undocumented(m_ostream, "HL", get_next_word());
                break;
            case RLD:
                print_rld(m_ostream);
                break;
            case IN_MC:
                print_in_undocumented(m_ostream, "(C)");
                break;
            case OUT_MC_0:
                print_out_Mr_r_undocumented(m_ostream, "C", "0");
                break;
            case LD_Mnn_SP:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "SP");
                break;
            case ADC_HL_SP:
                print_adc_r_s(m_ostream, "HL", "SP");
                break;
            case LD_SP_Mnn:
                print_ld(m_ostream, "SP", get_next_word());
                break;
            case LDI:
                print_ldi(m_ostream);
                break;
            case CPI:
                print_cpi(m_ostream);
                break;
            case INI:
                print_ini(m_ostream);
                break;
            case OUTI:
                print_outi(m_ostream);
                break;
            case LDD:
                print_ldd(m_ostream);
                break;
            case CPD:
                print_cpd(m_ostream);
                break;
            case IND:
                print_ind(m_ostream);
                break;
            case OUTD:
                print_outd(m_ostream);
                break;
            case LDIR:
                print_ldir(m_ostream);
                break;
            case CPIR:
                print_cpir(m_ostream);
                break;
            case INIR:
                print_inir(m_ostream);
                break;
            case OTIR:
                print_otir(m_ostream);
                break;
            case LDDR:
                print_lddr(m_ostream);
                break;
            case CPDR:
                print_cpdr(m_ostream);
                break;
            case INDR:
                print_indr(m_ostream);
                break;
            case OTDR:
                print_otdr(m_ostream);
                break;
            default:
                m_ostream << hexify(extd_opcode) << " (data)";
                break;
        }
    }

    NextByte DisassemblerZ80::get_next_byte() {
        return {
                .farg = m_memory.read(m_pc++)
        };
    }

    NextWord DisassemblerZ80::get_next_word() {
        return {
                .farg = m_memory.read(m_pc++),
                .sarg = m_memory.read(m_pc++)
        };
    }
}
