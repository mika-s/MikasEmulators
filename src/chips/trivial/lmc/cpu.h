#pragma once

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "flags.h"
#include <vector>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::lmc {
class InObserver;
enum class Opcode;
enum class OutType;
}
namespace emu::lmc {
class OutObserver;
}

namespace emu::lmc {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;

class Cpu {
public:
    Cpu(
        EmulatorMemory<Address, Data>& memory,
        Address initial_pc);

    ~Cpu();

    [[nodiscard]] auto can_run_next_instruction() const -> bool;

    void next_instruction();

    void reset_state();

    void start();

    void stop();

    void add_out_observer(OutObserver& observer);

    void remove_out_observer(OutObserver* observer);

    void add_in_observer(InObserver& observer);

    void remove_in_observer(InObserver* observer);

    auto memory() -> EmulatorMemory<Address, Data>&;

    [[nodiscard]] auto a() const -> Data;

    [[nodiscard]] auto f() const -> u8;

    [[nodiscard]] auto pc() const -> Address;

    void input(Data value);

private:
    bool m_is_halted;

    EmulatorMemory<Address, Data>& m_memory;

    Address m_pc;
    Data m_acc_reg;
    Flags m_flag_reg;

    std::vector<OutObserver*> m_out_observers;
    std::vector<InObserver*> m_in_observers;

    auto get_next_value() -> Data;

    static auto find_argument(Data raw_opcode) -> Address;

    static auto find_opcode(Data raw_opcode) -> Opcode;

    void notify_out_observers(Data acc_reg, OutType out_type);

    void notify_in_observers();

    void print_debug(Data opcode);
};
}
