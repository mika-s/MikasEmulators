#ifndef MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H
#define MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H

#include <vector>
#include <string>
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "z80/next_word.h"
#include "z80/next_byte.h"
#include "crosscutting/typedefs.h"

#define NOP           0x00
#define LXI_B         0x01
#define STAX_B        0x02
#define INX_B         0x03
#define INR_B         0x04
#define DCR_B         0x05
#define MVI_B         0x06
#define RLC_B         0x07
#define UNUSED_NOP_1  0x08
#define DAD_B         0x09
#define LDAX_B        0x0A
#define DCX_B         0x0B
#define INR_C         0x0C
#define DCR_C         0x0D
#define MVI_C         0x0E
#define RRC           0x0F
#define UNUSED_NOP_2  0x10
#define LXI_D         0x11
#define STAX_D        0x12
#define INX_D         0x13
#define INR_D         0x14
#define DCR_D         0x15
#define MVI_D         0x16
#define RAL           0x17
#define UNUSED_NOP_3  0x18
#define DAD_D         0x19
#define LDAX_D        0x1A
#define DCX_D         0x1B
#define INR_E         0x1C
#define DCR_E         0x1D
#define MVI_E         0x1E
#define RAR           0x1F
#define UNUSED_NOP_4  0x20
#define LXI_H         0x21
#define SHLD          0x22
#define INX_H         0x23
#define INR_H         0x24
#define DCR_H         0x25
#define MVI_H         0x26
#define DAA           0x27
#define UNUSED_NOP_5  0x28
#define DAD_H         0x29
#define LHLD          0x2A
#define DCX_H         0x2B
#define INR_L         0x2C
#define DCR_L         0x2D
#define MVI_L         0x2E
#define CMA           0x2F
#define UNUSED_NOP_6  0x30
#define LXI_SP        0x31
#define STA           0x32
#define INX_SP        0x33
#define INR_M         0x34
#define DCR_M         0x35
#define MVI_M         0x36
#define STC           0x37
#define UNUSED_NOP_7  0x38
#define DAD_SP        0x39
#define LDA           0x3A
#define DCX_SP        0x3B
#define INR_A         0x3C
#define DCR_A         0x3D
#define MVI_A         0x3E
#define CMC           0x3F
#define MOV_B_B       0x40
#define MOV_B_C       0x41
#define MOV_B_D       0x42
#define MOV_B_E       0x43
#define MOV_B_H       0x44
#define MOV_B_L       0x45
#define MOV_B_M       0x46
#define MOV_B_A       0x47
#define MOV_C_B       0x48
#define MOV_C_C       0x49
#define MOV_C_D       0x4A
#define MOV_C_E       0x4B
#define MOV_C_H       0x4C
#define MOV_C_L       0x4D
#define MOV_C_M       0x4E
#define MOV_C_A       0x4F
#define MOV_D_B       0x50
#define MOV_D_C       0x51
#define MOV_D_D       0x52
#define MOV_D_E       0x53
#define MOV_D_H       0x54
#define MOV_D_L       0x55
#define MOV_D_M       0x56
#define MOV_D_A       0x57
#define MOV_E_B       0x58
#define MOV_E_C       0x59
#define MOV_E_D       0x5A
#define MOV_E_E       0x5B
#define MOV_E_H       0x5C
#define MOV_E_L       0x5D
#define MOV_E_M       0x5E
#define MOV_E_A       0x5F
#define MOV_H_B       0x60
#define MOV_H_C       0x61
#define MOV_H_D       0x62
#define MOV_H_E       0x63
#define MOV_H_H       0x64
#define MOV_H_L       0x65
#define MOV_H_M       0x66
#define MOV_H_A       0x67
#define MOV_L_B       0x68
#define MOV_L_C       0x69
#define MOV_L_D       0x6A
#define MOV_L_E       0x6B
#define MOV_L_H       0x6C
#define MOV_L_L       0x6D
#define MOV_L_M       0x6E
#define MOV_L_A       0x6F
#define MOV_M_B       0x70
#define MOV_M_C       0x71
#define MOV_M_D       0x72
#define MOV_M_E       0x73
#define MOV_M_H       0x74
#define MOV_M_L       0x75
#define HLT           0x76
#define MOV_M_A       0x77
#define MOV_A_B       0x78
#define MOV_A_C       0x79
#define MOV_A_D       0x7A
#define MOV_A_E       0x7B
#define MOV_A_H       0x7C
#define MOV_A_L       0x7D
#define MOV_A_M       0x7E
#define MOV_A_A       0x7F
#define ADD_B         0x80
#define ADD_C         0x81
#define ADD_D         0x82
#define ADD_E         0x83
#define ADD_H         0x84
#define ADD_L         0x85
#define ADD_M         0x86
#define ADD_A         0x87
#define ADC_B         0x88
#define ADC_C         0x89
#define ADC_D         0x8A
#define ADC_E         0x8B
#define ADC_H         0x8C
#define ADC_L         0x8D
#define ADC_M         0x8E
#define ADC_A         0x8F
#define SUB_B         0x90
#define SUB_C         0x91
#define SUB_D         0x92
#define SUB_E         0x93
#define SUB_H         0x94
#define SUB_L         0x95
#define SUB_M         0x96
#define SUB_A         0x97
#define SBB_B         0x98
#define SBB_C         0x99
#define SBB_D         0x9A
#define SBB_E         0x9B
#define SBB_H         0x9C
#define SBB_L         0x9D
#define SBB_M         0x9E
#define SBB_A         0x9F
#define ANA_B         0xA0
#define ANA_C         0xA1
#define ANA_D         0xA2
#define ANA_E         0xA3
#define ANA_H         0xA4
#define ANA_L         0xA5
#define ANA_M         0xA6
#define ANA_A         0xA7
#define XRA_B         0xA8
#define XRA_C         0xA9
#define XRA_D         0xAA
#define XRA_E         0xAB
#define XRA_H         0xAC
#define XRA_L         0xAD
#define XRA_M         0xAE
#define XRA_A         0xAF
#define ORA_B         0xB0
#define ORA_C         0xB1
#define ORA_D         0xB2
#define ORA_E         0xB3
#define ORA_H         0xB4
#define ORA_L         0xB5
#define ORA_M         0xB6
#define ORA_A         0xB7
#define CMP_B         0xB8
#define CMP_C         0xB9
#define CMP_D         0xBA
#define CMP_E         0xBB
#define CMP_H         0xBC
#define CMP_L         0xBD
#define CMP_M         0xBE
#define CMP_A         0xBF
#define RNZ           0xC0
#define POP_B         0xC1
#define JNZ           0xC2
#define JMP           0xC3
#define CNZ           0xC4
#define PUSH_B        0xC5
#define ADI           0xC6
#define RST_0         0xC7
#define RZ            0xC8
#define RET           0xC9
#define JZ            0xCA
#define UNUSED_JMP_1  0xCB
#define CZ            0xCC
#define CALL          0xCD
#define ACI           0xCE
#define RST_1         0xCF
#define RNC           0xD0
#define POP_D         0xD1
#define JNC           0xD2
#define OUT           0xD3
#define CNC           0xD4
#define PUSH_D        0xD5
#define SUI           0xD6
#define RST_2         0xD7
#define RC            0xD8
#define UNUSED_RET_1  0xD9
#define JC            0xDA
#define IN            0xDB
#define CC            0xDC
#define UNUSED_CALL_1 0xDD
#define SBI           0xDE
#define RST_3         0xDF
#define RPO           0xE0
#define POP_H         0xE1
#define JPO           0xE2
#define XTHL          0xE3
#define CPO           0xE4
#define PUSH_H        0xE5
#define ANI           0xE6
#define RST_4         0xE7
#define RPE           0xE8
#define PCHL          0xE9
#define JPE           0xEA
#define XCHG          0xEB
#define CPE           0xEC
#define UNUSED_CALL_2 0xED
#define XRI           0xEE
#define RST_5         0xEF
#define RP            0xF0
#define POS_PSW       0xF1
#define JP            0xF2
#define DI            0xF3
#define CP            0xF4
#define PUSH_PSW      0xF5
#define ORI           0xF6
#define RST_6         0xF7
#define RM            0xF8
#define SPHL          0xF9
#define JM            0xFA
#define EI            0xFB
#define CM            0xFC
#define UNUSED_CALL_3 0xFD
#define CPI           0xFE
#define RST_7         0xFF

// @formatter:off
namespace emu::z80 {

    void aci(u8 &acc_reg, NextByte args, Flags &flag_reg, unsigned long &cycles);
    void adc(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void adc(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void add(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void add(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void adi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void ana(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void ana(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void ani(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void cc(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cm(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cma(u8 &acc_reg, unsigned long &cycles);
    void cmc(Flags &flag_reg, unsigned long &cycles);
    void cmp(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void cmp(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void cnc(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cnz(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cp(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cpe(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cpi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void cpo(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void cz(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void daa(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void dad(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, unsigned long &cycles);
    void dcr(u8 &reg, Flags &flag_reg, unsigned long &cycles);
    void dcr(u8 &reg, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void dcx(u8 &reg1, u8 &reg2, unsigned long &cycles);
    void dcx_sp(u16 &sp, unsigned long &cycles);
    void di(bool &inte, unsigned long &cycles);
    void ei(bool &inte, unsigned long &cycles);
    void hlt(bool &stopped, unsigned long &cycles);
    void in(u8 &acc_reg, const NextByte &args, std::vector<u8> io, unsigned long &cycles);
    void inr(u8 &reg, Flags &flag_reg, unsigned long &cycles);
    void inr(u8 &reg, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void inx(u8 &reg1, u8 &reg2, unsigned long &cycles);
    void inx_sp(u16 &sp, unsigned long &cycles);
    void jc(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jm(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jmp(u16 &pc, const NextWord &args, unsigned long &cycles);
    void jnc(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jnz(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jpe(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jpo(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jz(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void lda(u8 &acc_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ldax(u8 &acc_reg, u8 reg1, u8 reg2, const EmulatorMemory &memory, unsigned long &cycles);
    void lhld(u8 &l_reg, u8 &h_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void lxi(u8 &reg1, u8 &reg2, const NextWord &args, unsigned long &cycles);
    void lxi_sp(u16 &sp, const NextWord &args, unsigned long &cycles);
    void mov(u8 &to, u8 value, unsigned long &cycles);
    void mov(u8 &to, u8 value, unsigned long &cycles, bool is_memory_involved);
    void mvi(u8 &reg, const NextByte &args, unsigned long &cycles);
    void mvi(u8 &reg, const NextByte &args, unsigned long &cycles, bool is_memory_involved);
    void nop(unsigned long &cycles);
    void ora(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void ora(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void ori(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void out(u8 acc_reg, const NextByte &args, std::vector<u8> &io, unsigned long &cycles);
    void pchl(u16 &pc, u16 address, unsigned long &cycles);
    void pop(u8 &reg1, u8 &reg2, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void pop_psw(Flags &flag_reg, u8 &acc_reg, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void push(u8 reg1, u8 reg2, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void push_psw(const Flags &flag_reg, u8 &acc_reg, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void ral(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rar(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rc(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret(u16 &pc, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void rlc(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rm(u16 &pc, u16 &sp, EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rnc(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rnz(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rp(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rpe(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rpo(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rrc(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rst_0(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_1(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_2(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_3(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_4(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_5(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_6(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_7(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rz(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void sbb(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void sbb(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void sbi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void shld(u8 l_reg, u8 h_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void sphl(u16 &sp, u16 address, unsigned long &cycles);
    void sta(u8 &acc_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void stax(u8 acc_reg, u8 reg1, u8 reg2, EmulatorMemory &memory, unsigned long &cycles);
    void stc(Flags &flag_reg, unsigned long &cycles);
    void sub(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void sub(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void sui(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void unused_1(u8 opcode, unsigned long &cycles);
    void unused_3(u8 opcode, u16 &pc, unsigned long &cycles);
    void xchg(u8 &h_reg, u8 &l_reg, u8 &d_reg, u8 &e_reg, unsigned long &cycles);
    void xra(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void xra(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved);
    void xri(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void xthl(u8 &h_reg, u8 &l_reg, u8 &sp0, u8 &sp1, unsigned long &cycles);

    void print_aci(std::ostream &ostream, const NextByte &args);
    void print_adc(std::ostream &ostream, const std::string &reg);
    void print_add(std::ostream &ostream, const std::string &reg);
    void print_adi(std::ostream &ostream, const NextByte &args);
    void print_ana(std::ostream &ostream, const std::string &reg);
    void print_ani(std::ostream &ostream, const NextByte &args);
    void print_call(std::ostream &ostream, const NextWord &args);
    void print_cc(std::ostream &ostream, const NextWord &args);
    void print_cm(std::ostream &ostream, const NextWord &args);
    void print_cma(std::ostream &ostream);
    void print_cmc(std::ostream &ostream);
    void print_cmp(std::ostream &ostream, const std::string &reg);
    void print_cnc(std::ostream &ostream, const NextWord &args);
    void print_cnz(std::ostream &ostream, const NextWord &args);
    void print_cp(std::ostream &ostream, const NextWord &args);
    void print_cpe(std::ostream &ostream, const NextWord &args);
    void print_cpi(std::ostream &ostream, const NextByte &args);
    void print_cpo(std::ostream &ostream, const NextWord &args);
    void print_cz(std::ostream &ostream, const NextWord &args);
    void print_daa(std::ostream &ostream);
    void print_dad(std::ostream &ostream, const std::string &reg);
    void print_di(std::ostream &ostream);
    void print_dcr(std::ostream &ostream, const std::string &reg);
    void print_dcx(std::ostream &ostream, const std::string &reg);
    void print_ei(std::ostream &ostream);
    void print_hlt(std::ostream &ostream);
    void print_in(std::ostream &ostream, const NextByte &args);
    void print_jc(std::ostream &ostream, const NextWord &args);
    void print_jz(std::ostream &ostream, const NextWord &args);
    void print_jm(std::ostream &ostream, const NextWord &args);
    void print_jmp(std::ostream &ostream, const NextWord &args);
    void print_jnc(std::ostream &ostream, const NextWord &args);
    void print_jnz(std::ostream &ostream, const NextWord &args);
    void print_jp(std::ostream &ostream, const NextWord &args);
    void print_jpe(std::ostream &ostream, const NextWord &args);
    void print_jpo(std::ostream &ostream, const NextWord &args);
    void print_lda(std::ostream &ostream, const NextWord &args);
    void print_ldax(std::ostream &ostream, const std::string &reg);
    void print_lhld(std::ostream &ostream, const NextWord &args);
    void print_lxi(std::ostream &ostream, const std::string &reg, const NextWord &args);
    void print_inr(std::ostream &ostream, const std::string &reg);
    void print_inx(std::ostream &ostream, const std::string &reg);
    void print_mvi(std::ostream &ostream, const std::string &reg, const NextByte &args);
    void print_mov(std::ostream &ostream, const std::string &reg1, const std::string &reg2);
    void print_nop(std::ostream &ostream);
    void print_ora(std::ostream &ostream, const std::string &reg);
    void print_ori(std::ostream &ostream, const NextByte &args);
    void print_pchl(std::ostream &ostream);
    void print_pop(std::ostream &ostream, const std::string &reg);
    void print_push(std::ostream &ostream, const std::string &reg);
    void print_out(std::ostream &ostream, const NextByte &args);
    void print_ral(std::ostream &ostream);
    void print_rar(std::ostream &ostream);
    void print_rc(std::ostream &ostream);
    void print_ret(std::ostream &ostream);
    void print_rlc(std::ostream &ostream, const std::string &reg);
    void print_rm(std::ostream &ostream);
    void print_rnc(std::ostream &ostream);
    void print_rnz(std::ostream &ostream);
    void print_rp(std::ostream &ostream);
    void print_rpe(std::ostream &ostream);
    void print_rpo(std::ostream &ostream);
    void print_rst(std::ostream &ostream, int number);
    void print_rrc(std::ostream &ostream);
    void print_rz(std::ostream &ostream);
    void print_sbb(std::ostream &ostream, const std::string &reg);
    void print_sbi(std::ostream &ostream, const NextByte &args);
    void print_shld(std::ostream &ostream, const NextWord &args);
    void print_sphl(std::ostream &ostream);
    void print_sta(std::ostream &ostream, const NextWord &args);
    void print_stax(std::ostream &ostream, const std::string &reg);
    void print_stc(std::ostream &ostream);
    void print_sub(std::ostream &ostream, const std::string &reg);
    void print_sui(std::ostream &ostream, const NextByte &args);
    void print_unused_nop(std::ostream &ostream);
    void print_unused_jmp(std::ostream &ostream, const NextWord &args);
    void print_unused_ret(std::ostream &ostream);
    void print_unused_call(std::ostream &ostream, const NextWord &args);
    void print_xchg(std::ostream &ostream);
    void print_xra(std::ostream &ostream, const std::string &reg);
    void print_xri(std::ostream &ostream, const NextByte &args);
    void print_xthl(std::ostream &ostream);
}
// @formatter:on

#endif //MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H
