#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"

constexpr unsigned int HALT = 0x00;
constexpr unsigned int SET = 0x01;
constexpr unsigned int PUSH = 0x02;
constexpr unsigned int POP = 0x03;
constexpr unsigned int EQ = 0x04;
constexpr unsigned int GT = 0x05;
constexpr unsigned int JMP = 0x06;
constexpr unsigned int JT = 0x07;
constexpr unsigned int JF = 0x08;
constexpr unsigned int ADD = 0x09;
constexpr unsigned int MULT = 0x0A;
constexpr unsigned int MOD = 0x0B;
constexpr unsigned int AND = 0x0C;
constexpr unsigned int OR = 0x0D;
constexpr unsigned int NOT = 0x0E;
constexpr unsigned int RMEM = 0x0F;
constexpr unsigned int WMEM = 0x10;
constexpr unsigned int CALL = 0x11;
constexpr unsigned int RET = 0x12;
constexpr unsigned int OUT = 0x13;
constexpr unsigned int IN = 0x14;
constexpr unsigned int NOOP = 0x15;

namespace emu::synacor {

using emu::memory::EmulatorMemory;

void add();
void and_();
void call();
void eq();
void gt();
void halt(bool& is_halted);
void in();
void jf();
void jmp();
void jt();
void mod();
void mult();
void noop();
void not_();
void or_();
void out();
void pop();
void push();
void ret();
void rmem();
void set(Data &a, Data b);
void wmem();

void print_add(std::ostream& ostream);
void print_and(std::ostream& ostream);
void print_call(std::ostream& ostream);
void print_eq(std::ostream& ostream);
void print_gt(std::ostream& ostream);
void print_halt(std::ostream& ostream);
void print_in(std::ostream& ostream);
void print_jf(std::ostream& ostream);
void print_jmp(std::ostream& ostream);
void print_jt(std::ostream& ostream);
void print_mod(std::ostream& ostream);
void print_mult(std::ostream& ostream);
void print_noop(std::ostream& ostream);
void print_not(std::ostream& ostream);
void print_or(std::ostream& ostream);
void print_out(std::ostream& ostream);
void print_pop(std::ostream& ostream);
void print_push(std::ostream& ostream);
void print_ret(std::ostream& ostream);
void print_rmem(std::ostream& ostream);
void print_set(std::ostream& ostream, Data a, Data b);
void print_wmem(std::ostream& ostream);
}
