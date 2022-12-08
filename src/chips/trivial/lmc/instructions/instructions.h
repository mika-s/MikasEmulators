#pragma once

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "flags.h"

namespace emu::lmc {

void add(Data& acc_reg, Address address, EmulatorMemory<Address, Data> const& memory);
void sub(Data& acc_reg, Address address, EmulatorMemory<Address, Data> const& memory, Flags& flag_reg);
void sta(Data acc_reg, Address address, EmulatorMemory<Address, Data>& memory);
void lda(Data& acc_reg, Address address, EmulatorMemory<Address, Data> const& memory);
void bra(Address& pc, Address address);
void brz(Data acc_reg, Address& pc, Address address);
void brp(Address& pc, Address address, Flags flag_reg);
void hlt(bool& is_halted);

void print_add(std::ostream& ostream, Address address);
void print_bra(std::ostream& ostream, Address address);
void print_brp(std::ostream& ostream, Address address);
void print_brz(std::ostream& ostream, Address address);
void print_hlt(std::ostream& ostream);
void print_lda(std::ostream& ostream, Address address);
void print_sta(std::ostream& ostream, Address address);
void print_sub(std::ostream& ostream, Address address);
}
