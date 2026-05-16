#pragma once

#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include <stack>
#include <vector>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::synacor {
class InObserver;
enum class OutType;
}
namespace emu::synacor {
class OutObserver;
}

namespace emu::synacor {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;

class Cpu {
public:
    Cpu(
        EmulatorMemory<Address, RawData>& memory,
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

    auto memory() -> EmulatorMemory<Address, RawData>&;

    [[nodiscard]] auto pc() const -> Address;

    [[nodiscard]] auto r0() const -> RawData;

    void input(Data value);

private:
    bool m_is_halted;

    EmulatorMemory<Address, RawData>& m_memory;

    Data m_opcode { 0 };
    Address m_pc;
    std::stack<Address> m_stack;

    std::vector<OutObserver*> m_out_observers;
    std::vector<InObserver*> m_in_observers;

    auto get_next_value() -> RawData;

    void notify_out_observers(Data character);

    void notify_in_observers() const;

    void print_debug(Data opcode) const;
};
}
