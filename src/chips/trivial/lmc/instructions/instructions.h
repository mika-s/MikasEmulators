#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "flags.h"

namespace emu::lmc {

    void add(u16 &acc_reg, u8 address, const EmulatorMemory<u8, u16> &memory);
    void sub(u16 &acc_reg, u8 address, const EmulatorMemory<u8, u16> &memory, Flags &flag_reg);
    void sta(u16 acc_reg, u8 address, EmulatorMemory<u8, u16> &memory);
    void lda(u16 &acc_reg, u8 address, const EmulatorMemory<u8, u16> &memory);
    void bra(u8 &pc, u8 address);
    void brz(u16 acc_reg, u8 &pc, u8 address);
    void brp(u8 &pc, u8 address, Flags flag_reg);
    void hlt(bool &is_halted);
}
