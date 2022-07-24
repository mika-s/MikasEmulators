#include <iostream>
#include "disassemblerZ80.h"
#include "z80/instructions/instructions.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::exceptions::UnrecognizedOpcodeException;
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
                print_adc(m_ostream, "A", "B");
                break;
            case ADC_A_C:
                print_adc(m_ostream, "A", "C");
                break;
            case ADC_A_D:
                print_adc(m_ostream, "A", "D");
                break;
            case ADC_A_E:
                print_adc(m_ostream, "A", "E");
                break;
            case ADC_A_H:
                print_adc(m_ostream, "A", "H");
                break;
            case ADC_A_L:
                print_adc(m_ostream, "A", "L");
                break;
            case ADC_A_MHL:
                print_adc(m_ostream, "A", "(HL)");
                break;
            case ADC_A_A:
                print_adc(m_ostream, "A", "A");
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
                print_sbc(m_ostream, "A", "B");
                break;
            case SBC_A_C:
                print_sbc(m_ostream, "A", "C");
                break;
            case SBC_A_D:
                print_sbc(m_ostream, "A", "D");
                break;
            case SBC_A_E:
                print_sbc(m_ostream, "A", "E");
                break;
            case SBC_A_H:
                print_sbc(m_ostream, "A", "H");
                break;
            case SBC_A_L:
                print_sbc(m_ostream, "A", "L");
                break;
            case SBC_A_MHL:
                print_sbc(m_ostream, "A", "(HL)");
                break;
            case SBC_A_A:
                print_sbc(m_ostream, "A", "A");
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
                print_jp(m_ostream, get_next_word(), "NZ");
                break;
            case JP:
                print_jp(m_ostream, get_next_word());
                break;
            case CALL_NZ:
                print_call(m_ostream, get_next_word(), "NZ");
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
                print_jp(m_ostream, get_next_word(), "Z");
                break;
            case BITS:
                print_unused_jmp(m_ostream, get_next_word());
                break;
            case CALL_Z:
                print_call(m_ostream, get_next_word(), "Z");
                break;
            case CALL:
                print_call(m_ostream, get_next_word());
                break;
            case ADC_A_n:
                print_adc(m_ostream, "A", get_next_byte());
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
                print_jp(m_ostream, get_next_word(), "NC");
                break;
            case OUT:
                print_out(m_ostream, get_next_byte());
                break;
            case CALL_NC:
                print_call(m_ostream, get_next_word(), "NC");
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
                print_jp(m_ostream, get_next_word(), "C");
                break;
            case IN:
                print_in(m_ostream, get_next_byte());
                break;
            case CALL_C:
                print_call(m_ostream, get_next_word(), "C");
                break;
            case IX:
                print_next_ixy_instruction(get_next_byte().farg, "IX");
                break;
            case SBC_A_n:
                print_sbc(m_ostream, "A", get_next_byte());
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
                print_jp(m_ostream, get_next_word(), "PO");
                break;
            case EX_MSP_HL:
                print_ex(m_ostream, "(SP)", "HL");
                break;
            case CALL_PO:
                print_call(m_ostream, get_next_word(), "PO");
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
                print_jp(m_ostream, "(HL)");
                break;
            case JP_PE:
                print_jp(m_ostream, get_next_word(), "PE");
                break;
            case EX_DE_HL:
                print_ex(m_ostream, "DE", "HL");
                break;
            case CALL_PE:
                print_call(m_ostream, get_next_word(), "PE");
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
                print_jp(m_ostream, get_next_word(), "P");
                break;
            case DI:
                print_di(m_ostream);
                break;
            case CALL_P:
                print_call(m_ostream, get_next_word(), "P");
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
                print_jp(m_ostream, get_next_word(), "M");
                break;
            case EI:
                print_ei(m_ostream);
                break;
            case CALL_M:
                print_call(m_ostream, get_next_word(), "M");
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

    void DisassemblerZ80::print_next_ixy_instruction(u8 ixy_opcode, const std::string &ixy_reg) {
        switch (ixy_opcode) {
            case ADD_IXY_BC:
                print_add(m_ostream, ixy_reg, "BC");
                break;
            case ADD_IXY_DE:
                print_add(m_ostream, ixy_reg, "DE");
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
            case INC_IXH_UNDOC1:
                print_inc(m_ostream, "IXH");
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
            case INC_IXL_UNDOC1:
                print_inc(m_ostream, "IXL");
                break;
            case 0x32:
                std::cout << "0x32 (data)";
                break;
            case INC_MIXY_P_n:
                print_inc_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case ADD_IX_SP:
                print_add(m_ostream, "IX", "SP");
                break;
            case LD_B_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "B", ixy_reg, get_next_byte());
                break;
            case LD_C_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "C", ixy_reg, get_next_byte());
                break;
            case LD_D_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "D", ixy_reg, get_next_byte());
                break;
            case LD_E_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "E", ixy_reg, get_next_byte());
                break;
            case LD_H_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "H", ixy_reg, get_next_byte());
                break;
            case LD_L_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "L", ixy_reg, get_next_byte());
                break;
            case LD_A_MIXY_P_n:
                print_ld_r_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case ADD_A_IXH:
                print_add(m_ostream, "A", "IXH");
                break;
            case ADD_A_MIXY_P_n:
                print_add_MixyPn(m_ostream, "A", ixy_reg, get_next_byte());
                break;
            case SUB_IXH_UNDOC1:
                print_sub(m_ostream, "IXH");
                break;
            case XOR_IXL_UNDOC1:
                print_xor_r(m_ostream, "IXL");
                break;
            case OR_MIXY_P_n:
                print_or_MixyPn(m_ostream, ixy_reg, get_next_byte());
                break;
            case IXY_BITS:
                print_next_ixy_bits_instruction(get_next_byte().farg, ixy_reg);
                break;
            case POP_IXY:
                print_pop(m_ostream, ixy_reg);
                break;
            case EX_MSP_IX:
                print_ex(m_ostream, "(SP)", "IX");
                break;
            case PUSH_IXY:
                print_push(m_ostream, ixy_reg);
                break;
            case JP_MIXY:
                print_jp(m_ostream, "(" + ixy_reg + ")");
                break;
            case LD_SP_IX:
                print_ld(m_ostream, "SP", "IX");
                break;
            default:
                throw UnrecognizedOpcodeException(ixy_opcode, "IX/IY instructions");
        }
    }

    void DisassemblerZ80::print_next_ixy_bits_instruction(u8 bits_opcode, const std::string &ixy_reg) {
        switch (bits_opcode) {
            case RLC_MIXY_P_n_B_UNDOC1:
                print_rlc_MixyPn_r(m_ostream, ixy_reg, get_next_byte(), "B");
                break;
            case RL_MIXY_P_n_B_UNDOC1:
                print_rl_MixyPn_r(m_ostream, ixy_reg, get_next_byte(), "B");
                break;
            default:
                throw UnrecognizedOpcodeException(bits_opcode, "BITS instructions");
        }
    }

    void DisassemblerZ80::print_next_extd_instruction(u8 extd_opcode) {
        switch (extd_opcode) {
            case SBC_HL_BC:
                print_sbc(m_ostream, "HL", "BC");
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
            case IM_0_1:
            case IM_0_2:
                print_im(m_ostream, 0);
                break;
            case ADC_HL_BC:
                print_adc(m_ostream, "HL", "BC");
                break;
            case LD_BC_Mnn:
                print_ld(m_ostream, "BC", get_next_word());
                break;
            case SBC_HL_DE:
                print_sbc(m_ostream, "HL", "DE");
                break;
            case SBC_HL_HL:
                print_sbc(m_ostream, "HL", "HL");
                break;
            case SBC_HL_SP:
                print_sbc(m_ostream, "HL", "SP");
                break;
            case IM_1_1:
            case IM_1_2:
                print_im(m_ostream, 1);
                break;
            case RRD:
                print_rrd(m_ostream);
                break;
            case LD_Mnn_sp:
                print_ld_Mnn_dd(m_ostream, get_next_word(), "SP");
                break;
            case LD_sp_Mnn:
                print_ld(m_ostream, "SP", get_next_word());
                break;
            case 0x84:
                std::cout << "0x84 (data)";
                break;
            case LDI:
                print_ldi(m_ostream);
                break;
            case CPI:
                print_cpi(m_ostream);
                break;
            case CPD:
                print_cpd(m_ostream);
                break;
            case LDIR:
                print_ldir(m_ostream);
                break;
            case 0xc9:
                std::cout << "0xc9 (data)";
                break;
            case 0xf5:
                std::cout << "0xf5 (data)";
                break;
            default:
                throw UnrecognizedOpcodeException(extd_opcode, "EXTD instructions");
        }
    }

    NextByte DisassemblerZ80::get_next_byte() {
        return {
                .farg = m_memory[m_pc++]
        };
    }

    NextWord DisassemblerZ80::get_next_word() {
        return {
                .farg = m_memory[m_pc++],
                .sarg = m_memory[m_pc++]
        };
    }
}
