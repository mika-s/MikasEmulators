#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "flags.h"

namespace emu::lmc {

    void add(Data &acc_reg, Address address, const EmulatorMemory<Address, Data> &memory);
    void sub(Data &acc_reg, Address address, const EmulatorMemory<Address, Data> &memory, Flags &flag_reg);
    void sta(Data acc_reg, Address address, EmulatorMemory<Address, Data> &memory);
    void lda(Data &acc_reg, Address address, const EmulatorMemory<Address, Data> &memory);
    void bra(Address &pc, Address address);
    void brz(Data acc_reg, Address &pc, Address address);
    void brp(Address &pc, Address address, Flags flag_reg);
    void hlt(bool &is_halted);
}
