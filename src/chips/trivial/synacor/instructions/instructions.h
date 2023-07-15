#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include <stack>

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

void add(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void and_(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void call(Address& pc, std::stack<Address>& stack, RawData a);
void eq(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void gt(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void halt(bool& is_halted);
void in(Address a, EmulatorMemory<Address, RawData>& memory);
void jf(Address& pc, EmulatorMemory<Address, RawData> const& memory, RawData a, RawData b);
void jmp(Address& pc, EmulatorMemory<Address, RawData> const& memory, RawData a);
void jt(Address& pc, EmulatorMemory<Address, RawData> const& memory, RawData a, RawData b);
void mod(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void mult(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void noop();
void not_(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b);
void or_(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b, RawData c);
void out(Data character);
void pop(std::stack<Address>& stack, EmulatorMemory<Address, RawData>& memory, RawData a);
void push(std::stack<Address>& stack, EmulatorMemory<Address, RawData> const& memory, RawData a);
void ret(std::stack<Address>& stack, Address& pc, bool& is_halted);
void rmem(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b);
void set(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b);
void wmem(EmulatorMemory<Address, RawData>& memory, RawData a, RawData b);

void print_add(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_and(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_call(std::ostream& ostream, RawData a);
void print_eq(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_gt(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_halt(std::ostream& ostream);
void print_in(std::ostream& ostream, RawData a);
void print_jf(std::ostream& ostream, RawData a, RawData b);
void print_jmp(std::ostream& ostream, RawData a);
void print_jt(std::ostream& ostream, RawData a, RawData b);
void print_mod(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_mult(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_noop(std::ostream& ostream);
void print_not(std::ostream& ostream, RawData a, RawData b);
void print_or(std::ostream& ostream, RawData a, RawData b, RawData c);
void print_out(std::ostream& ostream, RawData character);
void print_pop(std::ostream& ostream, RawData a);
void print_push(std::ostream& ostream, RawData a);
void print_ret(std::ostream& ostream);
void print_rmem(std::ostream& ostream, RawData a, RawData b);
void print_set(std::ostream& ostream, RawData a, RawData b);
void print_wmem(std::ostream& ostream, RawData a, RawData b);
}
