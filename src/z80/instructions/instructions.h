#ifndef MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H
#define MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H

#include <vector>
#include <string>
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/misc/next_word.h"
#include "crosscutting/typedefs.h"

// Main opcodes:
#define NOP           0x00
#define LD_BC_nn      0x01
#define LD_MBC_A      0x02
#define INC_BC        0x03
#define INC_B         0x04
#define DEC_B         0x05
#define LD_B_n        0x06
#define RLCA          0x07
#define EX_AF_AFP     0x08
#define ADD_HL_BC     0x09
#define LD_A_MBC      0x0A
#define DEC_BC        0x0B
#define INC_C         0x0C
#define DEC_C         0x0D
#define LD_C_n        0x0E
#define RRCA          0x0F
#define DJNZ          0x10
#define LD_DE_nn      0x11
#define LD_MDE_A      0x12
#define INC_DE        0x13
#define INC_D         0x14
#define DEC_D         0x15
#define LD_D_n        0x16
#define RLA           0x17
#define JR_e          0x18
#define ADD_HL_DE     0x19
#define LD_A_MDE      0x1A
#define DEC_DE        0x1B
#define INC_E         0x1C
#define DEC_E         0x1D
#define LD_E_n        0x1E
#define RRA           0x1F
#define JR_NZ_e       0x20
#define LD_HL_nn      0x21
#define LD_Mnn_HL     0x22
#define INC_HL        0x23
#define INC_H         0x24
#define DEC_H         0x25
#define LD_H_n        0x26
#define DAA           0x27
#define JR_Z_e        0x28
#define ADD_HL_HL     0x29
#define LD_HL_Mnn     0x2A
#define DEC_HL        0x2B
#define INC_L         0x2C
#define DEC_L         0x2D
#define LD_L_n        0x2E
#define CPL           0x2F
#define JR_NC_e       0x30
#define LD_SP_nn      0x31
#define LD_Mnn_A      0x32
#define INC_SP        0x33
#define INC_MHL       0x34
#define DEC_MHL       0x35
#define LD_MHL_n      0x36
#define SCF           0x37
#define JR_C_e        0x38
#define ADD_HL_SP     0x39
#define LD_A_Mnn      0x3A
#define DEC_SP        0x3B
#define INC_A         0x3C
#define DEC_A         0x3D
#define LD_A_n        0x3E
#define CCF           0x3F
#define LD_B_B        0x40
#define LD_B_C        0x41
#define LD_B_D        0x42
#define LD_B_E        0x43
#define LD_B_H        0x44
#define LD_B_L        0x45
#define LD_B_MHL      0x46
#define LD_B_A        0x47
#define LD_C_B        0x48
#define LD_C_C        0x49
#define LD_C_D        0x4A
#define LD_C_E        0x4B
#define LD_C_H        0x4C
#define LD_C_L        0x4D
#define LD_C_MHL      0x4E
#define LD_C_A        0x4F
#define LD_D_B        0x50
#define LD_D_C        0x51
#define LD_D_D        0x52
#define LD_D_E        0x53
#define LD_D_H        0x54
#define LD_D_L        0x55
#define LD_D_MHL      0x56
#define LD_D_A        0x57
#define LD_E_B        0x58
#define LD_E_C        0x59
#define LD_E_D        0x5A
#define LD_E_E        0x5B
#define LD_E_H        0x5C
#define LD_E_L        0x5D
#define LD_E_MHL      0x5E
#define LD_E_A        0x5F
#define LD_H_B        0x60
#define LD_H_C        0x61
#define LD_H_D        0x62
#define LD_H_E        0x63
#define LD_H_H        0x64
#define LD_H_L        0x65
#define LD_H_MHL      0x66
#define LD_H_A        0x67
#define LD_L_B        0x68
#define LD_L_C        0x69
#define LD_L_D        0x6A
#define LD_L_E        0x6B
#define LD_L_H        0x6C
#define LD_L_L        0x6D
#define LD_L_MHL      0x6E
#define LD_L_A        0x6F
#define LD_MHL_B      0x70
#define LD_MHL_C      0x71
#define LD_MHL_D      0x72
#define LD_MHL_E      0x73
#define LD_MHL_H      0x74
#define LD_MHL_L      0x75
#define HALT          0x76
#define LD_MHL_A      0x77
#define LD_A_B        0x78
#define LD_A_C        0x79
#define LD_A_D        0x7A
#define LD_A_E        0x7B
#define LD_A_H        0x7C
#define LD_A_L        0x7D
#define LD_A_MHL      0x7E
#define LD_A_A        0x7F
#define ADD_A_B       0x80
#define ADD_A_C       0x81
#define ADD_A_D       0x82
#define ADD_A_E       0x83
#define ADD_A_H       0x84
#define ADD_A_L       0x85
#define ADD_A_MHL     0x86
#define ADD_A_A       0x87
#define ADC_A_B       0x88
#define ADC_A_C       0x89
#define ADC_A_D       0x8A
#define ADC_A_E       0x8B
#define ADC_A_H       0x8C
#define ADC_A_L       0x8D
#define ADC_A_MHL     0x8E
#define ADC_A_A       0x8F
#define SUB_B         0x90
#define SUB_C         0x91
#define SUB_D         0x92
#define SUB_E         0x93
#define SUB_H         0x94
#define SUB_L         0x95
#define SUB_MHL       0x96
#define SUB_A         0x97
#define SBC_A_B       0x98
#define SBC_A_C       0x99
#define SBC_A_D       0x9A
#define SBC_A_E       0x9B
#define SBC_A_H       0x9C
#define SBC_A_L       0x9D
#define SBC_A_MHL     0x9E
#define SBC_A_A       0x9F
#define AND_B         0xA0
#define AND_C         0xA1
#define AND_D         0xA2
#define AND_E         0xA3
#define AND_H         0xA4
#define AND_L         0xA5
#define AND_MHL       0xA6
#define AND_A         0xA7
#define XOR_B         0xA8
#define XOR_C         0xA9
#define XOR_D         0xAA
#define XOR_E         0xAB
#define XOR_H         0xAC
#define XOR_L         0xAD
#define XOR_MHL       0xAE
#define XOR_A         0xAF
#define OR_B          0xB0
#define OR_C          0xB1
#define OR_D          0xB2
#define OR_E          0xB3
#define OR_H          0xB4
#define OR_L          0xB5
#define OR_MHL        0xB6
#define OR_A          0xB7
#define CP_B          0xB8
#define CP_C          0xB9
#define CP_D          0xBA
#define CP_E          0xBB
#define CP_H          0xBC
#define CP_L          0xBD
#define CP_MHL        0xBE
#define CP_A          0xBF
#define RET_NZ        0xC0
#define POP_BC        0xC1
#define JP_NZ         0xC2
#define JP            0xC3
#define CALL_NZ       0xC4
#define PUSH_BC       0xC5
#define ADD_A_n       0xC6
#define RST_0         0xC7
#define RET_Z         0xC8
#define RET           0xC9
#define JP_Z          0xCA
#define BITS          0xCB
#define CALL_Z        0xCC
#define CALL          0xCD
#define ADC_A_n       0xCE
#define RST_1         0xCF
#define RET_NC        0xD0
#define POP_DE        0xD1
#define JP_NC         0xD2
#define OUT           0xD3
#define CALL_NC       0xD4
#define PUSH_DE       0xD5
#define SUB_n         0xD6
#define RST_2         0xD7
#define RET_C         0xD8
#define EXX           0xD9
#define JP_C          0xDA
#define IN            0xDB
#define CALL_C        0xDC
#define IX            0xDD
#define SBC_A_n       0xDE
#define RST_3         0xDF
#define RET_PO        0xE0
#define POP_HL        0xE1
#define JP_PO         0xE2
#define EX_MSP_HL     0xE3
#define CALL_PO       0xE4
#define PUSH_HL       0xE5
#define AND_n         0xE6
#define RST_4         0xE7
#define RET_PE        0xE8
#define JP_MHL        0xE9
#define JP_PE         0xEA
#define EX_DE_HL      0xEB
#define CALL_PE       0xEC
#define EXTD          0xED
#define XOR_n         0xEE
#define RST_5         0xEF
#define RET_P         0xF0
#define POP_AF        0xF1
#define JP_P          0xF2
#define DI            0xF3
#define CALL_P        0xF4
#define PUSH_AF       0xF5
#define OR_n          0xF6
#define RST_6         0xF7
#define RET_M         0xF8
#define LD_SP_HL      0xF9
#define JP_M          0xFA
#define EI            0xFB
#define CALL_M        0xFC
#define IY            0xFD
#define CP_n          0xFE
#define RST_7         0xFF

// BITS opcodes:

#define RLC_B            0x00
#define RLC_C            0x01
#define RLC_D            0x02
#define RLC_E            0x03
#define RLC_H            0x04
#define RLC_L            0x05
#define RLC_A            0x07
#define BIT_0_B          0x40
#define BIT_0_C          0x41
#define BIT_0_D          0x42
#define BIT_0_E          0x43
#define BIT_0_H          0x44
#define BIT_0_L          0x45
#define BIT_0_MHL        0x46
#define BIT_0_A          0x47
#define BIT_1_B          0x48
#define BIT_1_C          0x49
#define BIT_1_D          0x4A
#define BIT_1_E          0x4B
#define BIT_1_H          0x4C
#define BIT_1_L          0x4D
#define BIT_1_MHL        0x4E
#define BIT_1_A          0x4F
#define BIT_2_B          0x50
#define BIT_2_C          0x51
#define BIT_2_D          0x52
#define BIT_2_E          0x53
#define BIT_2_H          0x54
#define BIT_2_L          0x55
#define BIT_2_MHL        0x56
#define BIT_2_A          0x57
#define BIT_3_B          0x58
#define BIT_3_C          0x59
#define BIT_3_D          0x5A
#define BIT_3_E          0x5B
#define BIT_3_H          0x5C
#define BIT_3_L          0x5D
#define BIT_3_MHL        0x5E
#define BIT_3_A          0x5F
#define BIT_4_B          0x60
#define BIT_4_C          0x61
#define BIT_4_D          0x62
#define BIT_4_E          0x63
#define BIT_4_H          0x64
#define BIT_4_L          0x65
#define BIT_4_MHL        0x66
#define BIT_4_A          0x67
#define BIT_5_B          0x68
#define BIT_5_C          0x69
#define BIT_5_D          0x6A
#define BIT_5_E          0x6B
#define BIT_5_H          0x6C
#define BIT_5_L          0x6D
#define BIT_5_MHL        0x6E
#define BIT_5_A          0x6F
#define BIT_6_B          0x70
#define BIT_6_C          0x71
#define BIT_6_D          0x72
#define BIT_6_E          0x73
#define BIT_6_H          0x74
#define BIT_6_L          0x75
#define BIT_6_MHL        0x76
#define BIT_6_A          0x77
#define BIT_7_B          0x78
#define BIT_7_C          0x79
#define BIT_7_D          0x7A
#define BIT_7_E          0x7B
#define BIT_7_H          0x7C
#define BIT_7_L          0x7D
#define BIT_7_MHL        0x7E
#define BIT_7_A          0x7F

// IX/IY opcodes:

#define ADD_IXY_BC       0x09
#define ADD_IXY_DE       0x19
#define LD_IXY_nn        0x21
#define LD_Mnn_IXY       0x22
#define INC_IXY          0x23
#define INC_IXH_UNDOC1   0x24
#define ADD_IXY_IXY      0x29
#define LD_IXY_Mnn       0x2A
#define DEC_IXY          0x2B
#define INC_IXL_UNDOC1   0x2C
#define INC_MIXY_P_n     0x34
#define ADD_IXY_SP       0x39
#define LD_B_MIXY_P_n    0x46
#define LD_C_MIXY_P_n    0x4E
#define LD_D_MIXY_P_n    0x56
#define LD_E_MIXY_P_n    0x5E
#define LD_H_MIXY_P_n    0x66
#define LD_L_MIXY_P_n    0x6E
#define LD_A_MIXY_P_n    0x7E
#define ADD_A_IXYH_UNDOC 0x84
#define ADD_A_IXYL_UNDOC 0x85
#define ADD_A_MIXY_P_n   0x86
#define ADC_A_IXYH_UNDOC 0x8C
#define ADC_A_IXYL_UNDOC 0x8D
#define ADC_A_MIXY_P_n   0x8E
#define SUB_IXYH_UNDOC1  0x94
#define SUB_IXYL_UNDOC1  0x95
#define SUB_MIXY_P_n     0x96
#define SBC_A_IXYH_UNDOC 0x9C
#define SBC_A_IXYL_UNDOC 0x9D
#define SBC_A_MIXY_P_n   0x9E
#define AND_IXYL_UNDOC1  0xA4
#define AND_IXYH_UNDOC1  0xA5
#define AND_MIXY_P_n     0xA6
#define XOR_IXYH_UNDOC1  0xAC
#define XOR_IXYL_UNDOC1  0xAD
#define XOR_MIXY_P_n     0xAE
#define OR_IXYL_UNDOC1   0xB4
#define OR_IXYH_UNDOC1   0xB5
#define OR_MIXY_P_n      0xB6
#define CP_IXYH_UNDOC1   0xBC
#define CP_IXYL_UNDOC1   0xBD
#define CP_MIXY_P_n      0xBE
#define IXY_BITS         0xCB
#define POP_IXY          0xE1
#define EX_MSP_IX        0xE3
#define PUSH_IXY         0xE5
#define JP_MIXY          0xE9
#define LD_SP_IXY        0xF9

// IXY BITS opcodes

#define RLC_MIXY_P_n_B_UNDOC1 0x00
#define RLC_MIXY_P_n_C_UNDOC1 0x01
#define RLC_MIXY_P_n_D_UNDOC1 0x02
#define RLC_MIXY_P_n_E_UNDOC1 0x03
#define RLC_MIXY_P_n_H_UNDOC1 0x04
#define RLC_MIXY_P_n_L_UNDOC1 0x05
#define RLC_MIXY_P_n          0x06
#define RLC_MIXY_P_n_A_UNDOC1 0x07
#define RL_MIXY_P_n_B_UNDOC1  0x10

// EXTD opcodes:

#define SBC_HL_BC     0x42
#define LD_Mnn_BC     0x43
#define NEG           0x44
#define IM_0_1        0x46
#define ADC_HL_BC     0x4A
#define LD_BC_Mnn     0x4B
#define NEG_UNDOC1    0x4C
#define SBC_HL_DE     0x52
#define NEG_UNDOC2    0x54
#define IM_1_1        0x56
#define NEG_UNDOC3    0x5C
#define ADC_HL_DE     0x5A
#define SBC_HL_HL     0x62
#define NEG_UNDOC4    0x64
#define IM_0_2        0x66
#define RRD           0x67
#define ADC_HL_HL     0x6A
#define NEG_UNDOC5    0x6C
#define SBC_HL_SP     0x72
#define LD_Mnn_sp     0x73
#define NEG_UNDOC6    0x74
#define IM_1_2        0x76
#define ADC_HL_SP     0x7A
#define LD_sp_Mnn     0x7B
#define NEG_UNDOC7    0x7C
#define LDI           0xA0
#define CPI           0xA1
#define CPD           0xA9
#define LDIR          0xB0
#define CPDR          0xB9

// BITS opcodes:

// @formatter:off
namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::misc::NextWord;

    void adc_A_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg_h_or_l, Flags &flag_reg, unsigned long &cycles);
    void adc_A_MixyPd(u8 &reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void adc_A_n(u8 &acc_reg, NextByte args, Flags &flag_reg, unsigned long &cycles);
    void adc_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void adc_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void adc_hl_ss(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, unsigned long &cycles);
    void add_A_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg, Flags &flag_reg, unsigned long &cycles);
    void add_A_MixyPd(u8 &reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void add_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void add_A_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void add_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void add_HL_ss(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, unsigned long &cycles);
    void add_ixy_pp(u16 &ix_reg, u16 value_to_add, Flags &flag_reg, unsigned long &cycles);
    void and_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void and_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void and_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void and_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void bit_MHL(unsigned int bit_number, u16 hl_reg, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void bit_r(unsigned int bit_number, u8 reg, Flags &flag_reg, unsigned long &cycles);
    void call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void call_c(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_m(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_nc(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_nz(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_p(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_pe(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_po(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void call_z(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void ccf(Flags &flag_reg, unsigned long &cycles);
    void cpd(u8 &b_reg, u8 &c_reg, u8 &h_reg, u8 &l_reg, u8 acc_reg, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void cpdr(u16 &pc, u8 &b_reg, u8 &c_reg, u8 &h_reg, u8 &l_reg, u8 acc_reg, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void cp_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void cp_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void cp_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void cp_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void cpl(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void daa(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void dec_r(u8 &reg, Flags &flag_reg, unsigned long &cycles);
    void dec_ss(u8 &reg1, u8 &reg2, unsigned long &cycles);
    void dec_sp(u16 &sp, unsigned long &cycles);
    void dec_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles);
    void dec_ixy(u16 &ix_or_iy, unsigned long &cycles);
    void di(bool &iff1, bool &iff2, unsigned long &cycles);
    void djnz(u8 &b_reg, u16 &pc, const NextByte &args, unsigned long &cycles);
    void ei(bool &iff1, bool &iff2, unsigned long &cycles);
    void ex(u8 &acc_reg, Flags &flag_reg, u8 &acc_p_reg, Flags &flag_p_reg, unsigned long &cycles);
    void ex_de_hl(u8 &h_reg, u8 &l_reg, u8 &d_reg, u8 &e_reg, unsigned long &cycles);
    void ex_msp_hl(u16 sp, EmulatorMemory &memory, u8 &h_reg, u8 &l_reg, unsigned long &cycles);
    void ex_msp_ixy(u16 sp, EmulatorMemory &memory, u16 &ixy_reg, unsigned long &cycles);
    void exx(u8 &b_reg, u8 &c_reg, u8 &b_p_reg, u8 &c_p_reg, u8 &d_reg, u8 &e_reg, u8 &d_p_reg, u8 &e_p_reg, u8 &h_reg, u8 &l_reg, u8 &h_p_reg, u8 &l_p_reg, unsigned long &cycles);
    void halt(bool &stopped, unsigned long &cycles);
    void in_A_Mn(u8 &acc_reg, const NextByte &args, std::vector<u8> io, unsigned long &cycles);
    void inc_r(u8 &reg, Flags &flag_reg, unsigned long &cycles);
    void inc_ss(u8 &reg1, u8 &reg2, unsigned long &cycles);
    void inc_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles);
    void inc_sp(u16 &sp, unsigned long &cycles);
    void inc_ixy(u16 &ixy_reg, unsigned long &cycles);
    void jp(u16 &pc, const NextWord &args, unsigned long &cycles);
    void jp_c(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_hl(u16 &pc, u16 address, unsigned long &cycles);
    void jp_m(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_nc(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_nz(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_p(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_pe(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_po(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_z(u16 &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles);
    void jp_ixy(u16 &pc, u16 ixy_reg, unsigned long &cycles);
    void jr(u16 &pc, const NextByte &args, unsigned long &cycles);
    void jr_c(u16 &pc, const NextByte &args, const Flags &flag_reg, unsigned long &cycles);
    void jr_nc(u16 &pc, const NextByte &args, const Flags &flag_reg, unsigned long &cycles);
    void jr_nz(u16 &pc, const NextByte &args, const Flags &flag_reg, unsigned long &cycles);
    void jr_z(u16 &pc, const NextByte &args, const Flags &flag_reg, unsigned long &cycles);
    void ldax(u8 &acc_reg, u8 reg1, u8 reg2, const EmulatorMemory &memory, unsigned long &cycles);
    void ld_r_n(u8 &reg, const NextByte &args, unsigned long &cycles);
    void ld_r_r(u8 &to, u8 value, unsigned long &cycles);
    void ld_r_MHL(u8 &reg, u8 value, unsigned long &cycles);
    void ld_A_Mss(u8 &to, u8 value, unsigned long &cycles);
    void ld_A_Mnn(u8 &reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_HL_Mnn(u8 &reg1, u8 &reg2, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_dd_nn(u8 &reg1, u8 &reg2, const NextWord &args, unsigned long &cycles);
    void ld_ixy_nn(u16 &ixy_reg, const NextWord &args, unsigned long &cycles);
    void ld_ixy_Mnn(u16 &ixy_reg, const NextByte &args, const EmulatorMemory &memory, unsigned long &cycles);
    void ld_r_MixyPd(u8 &reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, unsigned long &cycles);
    void ld_MHL_n(u8 &reg, const NextByte &args, unsigned long &cycles);
    void ld_MHL_r(u8 &to, u8 value, unsigned long &cycles);
    void ld_Mnn_A(u8 &acc_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_Mnn_HL(u8 l_reg, u8 h_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_Mnn_dd(u8 l_reg, u8 h_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_Mnn_sp(u16 sp, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles);
    void ld_Mnn_ixy(const NextByte &args, EmulatorMemory &memory, u16 ixy_reg, unsigned long &cycles);
    void ld_Mss_A(u8 &to, u8 acc_reg, unsigned long &cycles);
    void ld_sp_Mnn(u16 &sp, EmulatorMemory memory, const NextWord &args, unsigned long &cycles);
    void ld_sp_nn(u16 &sp, const NextWord &args, unsigned long &cycles);
    void ld_sp_hl(u16 &sp, u16 address, unsigned long &cycles);
    void ld_sp_ixy(u16 &sp, u16 ixy, unsigned long &cycles);
    void ldir(u16 &pc, u8 &b_reg, u8 &c_reg, u8 &d_reg, u8 &e_reg, u8 &h_reg, u8 &l_reg, u8 acc_reg, EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void neg(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void nop(unsigned long &cycles);
    void or_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void or_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void or_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void or_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void out_Mn_A(u8 acc_reg, const NextByte &args, std::vector<u8> &io, unsigned long &cycles);
    void pop(u8 &reg1, u8 &reg2, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void pop_af(Flags &flag_reg, u8 &acc_reg, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void pop_ixy(u16 &ix_iy_reg, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void push_qq(u8 reg1, u8 reg2, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void push_af(const Flags &flag_reg, u8 acc_reg, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void push_ixy(u16 ixy_reg, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void ret(u16 &pc, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles);
    void ret_c(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_m(u16 &pc, u16 &sp, EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_nc(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_nz(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_p(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_pe(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_po(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void ret_z(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles);
    void rla(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rlca(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rlc_r(u8 &reg, Flags &flag_reg, unsigned long &cycles);
    void rlc_MixyPd(u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void rlc_MixyPd_r(u8 &reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void rra(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rrca(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles);
    void rrd(u8 &acc_reg, u8 &value, Flags &flag_reg, unsigned long &cycles);
    void rst_0(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_1(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_2(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_3(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_4(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_5(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_6(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void rst_7(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles);
    void sbc_A_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg_h_or_l, Flags &flag_reg, unsigned long &cycles);
    void sbc_A_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void sbc_A_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void sbc_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void sbc_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void sbc_HL_ss(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, unsigned long &cycles);
    void scf(Flags &flag_reg, unsigned long &cycles);
    void sub_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);
    void sub_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void sub_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void sub_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void unused_1(u8 opcode, unsigned long &cycles);
    void unused_3(u8 opcode, u16 &pc, unsigned long &cycles);
    void xor_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles);
    void xor_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void xor_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles);
    void xor_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg, unsigned long &cycles);

    void print_adc(std::ostream &ostream, const std::string &dest, const std::string &src);
    void print_adc(std::ostream &ostream, const std::string &reg, const NextByte &args);
    void print_adc_MixyPn(std::ostream &ostream, const std::string &reg, const std::string &ixy_reg, const NextByte &args);
    void print_add(std::ostream &ostream, const std::string &dest, const std::string &src);
    void print_add_r_n(std::ostream &ostream, const std::string &reg, const NextByte &args);
    void print_and_r(std::ostream &ostream, const std::string &reg);
    void print_and_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_and_n(std::ostream &ostream, const NextByte &args);
    void print_add_MixyPn(std::ostream &ostream, const std::string &reg, const std::string &ixy_reg, const NextByte &args);
    void print_bit(std::ostream &ostream, unsigned int bit_number, const std::string &src);
    void print_call(std::ostream &ostream, const NextWord &args);
    void print_call(std::ostream &ostream, const NextWord &args, const std::string &condition);
    void print_ccf(std::ostream &ostream);
    void print_cp(std::ostream &ostream, const std::string &reg);
    void print_cp(std::ostream &ostream, const NextByte &args);
    void print_cp_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_cpd(std::ostream &ostream);
    void print_cpdr(std::ostream &ostream);
    void print_cpi(std::ostream &ostream);
    void print_cpl(std::ostream &ostream);
    void print_daa(std::ostream &ostream);
    void print_di(std::ostream &ostream);
    void print_djnz(std::ostream &ostream, const NextByte &args);
    void print_dec(std::ostream &ostream, const std::string &reg);
    void print_ei(std::ostream &ostream);
    void print_ex(std::ostream &ostream, const std::string& reg, const std::string& comp_reg);
    void print_exx(std::ostream &ostream);
    void print_halt(std::ostream &ostream);
    void print_im(std::ostream &ostream, unsigned int interrupt_mode);
    void print_in(std::ostream &ostream, const NextByte &args);
    void print_jp(std::ostream &ostream, const NextWord &args);
    void print_jp(std::ostream &ostream, const NextWord &args, const std::string& condition);
    void print_jp(std::ostream &ostream, const std::string& condition);
    void print_jr(std::ostream &ostream, const NextByte &args);
    void print_jr(std::ostream &ostream, const std::string &condition, const NextByte &args);
    void print_inc(std::ostream &ostream, const std::string &reg);
    void print_inc_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_ld(std::ostream &ostream, const std::string &dest, const std::string &src);
    void print_ld(std::ostream &ostream, const std::string &dest, const NextByte &args);
    void print_ld(std::ostream &ostream, const std::string &dest, const NextWord &args);
    void print_ld_dd_nn(std::ostream &ostream, const std::string &reg, const NextWord &args);
    void print_ld_Mnn_dd(std::ostream &ostream, const NextWord &args, const std::string &src);
    void print_ld_r_MixyPn(std::ostream &ostream, const std::string &dest, const std::string &ixy_reg, const NextByte &args);
    void print_ldi(std::ostream &ostream);
    void print_ldir(std::ostream &ostream);
    void print_neg(std::ostream &ostream);
    void print_nop(std::ostream &ostream);
    void print_or_r(std::ostream &ostream, const std::string &reg);
    void print_or_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_or_n(std::ostream &ostream, const NextByte &args);
    void print_pop(std::ostream &ostream, const std::string &reg);
    void print_push(std::ostream &ostream, const std::string &reg);
    void print_out(std::ostream &ostream, const NextByte &args);
    void print_ret(std::ostream &ostream);
    void print_ret(std::ostream &ostream, const std::string& condition);
    void print_rla(std::ostream &ostream);
    void print_rl(std::ostream &ostream, const std::string &reg);
    void print_rl_MixyPn_r(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args, const std::string &reg);
    void print_rlc(std::ostream &ostream, const std::string &reg);
    void print_rlc_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_rlc_MixyPn_r(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args, const std::string &reg);
    void print_rlca(std::ostream &ostream);
    void print_rst(std::ostream &ostream, int number);
    void print_rr(std::ostream &ostream, const std::string &reg);
    void print_rra(std::ostream &ostream);
    void print_rrc(std::ostream &ostream, const std::string &reg);
    void print_rrca(std::ostream &ostream);
    void print_rrd(std::ostream &ostream);
    void print_sbc(std::ostream &ostream, const std::string &dest, const std::string &src);
    void print_sbc(std::ostream &ostream, const std::string &reg, const NextByte &args);
    void print_sbc_MixyPn(std::ostream &ostream, const std::string &reg, const std::string &ixy_reg, const NextByte &args);
    void print_scf(std::ostream &ostream);
    void print_sub(std::ostream &ostream, const std::string &reg);
    void print_sub(std::ostream &ostream, const NextByte &args);
    void print_sub_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
    void print_unused_nop(std::ostream &ostream);
    void print_unused_jmp(std::ostream &ostream, const NextWord &args);
    void print_unused_ret(std::ostream &ostream);
    void print_unused_call(std::ostream &ostream, const NextWord &args);
    void print_xor_r(std::ostream &ostream, const std::string &reg);
    void print_xor_n(std::ostream &ostream, const NextByte &args);
    void print_xor_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args);
}
// @formatter:on

#endif //MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTIONS_H
