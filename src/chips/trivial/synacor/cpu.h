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
        EmulatorMemory<Address, Data>& memory,
        Address initial_pc);

    ~Cpu();

    [[nodiscard]] bool can_run_next_instruction() const;

    void next_instruction();

    void reset_state();

    void start();

    void stop();

    void add_out_observer(OutObserver& observer);

    void remove_out_observer(OutObserver* observer);

    void add_in_observer(InObserver& observer);

    void remove_in_observer(InObserver* observer);

    EmulatorMemory<Address, Data>& memory();

    [[nodiscard]] Data r0() const;

    [[nodiscard]] Data r1() const;

    [[nodiscard]] Data r2() const;

    [[nodiscard]] Data r3() const;

    [[nodiscard]] Data r4() const;

    [[nodiscard]] Data r5() const;

    [[nodiscard]] Data r6() const;

    [[nodiscard]] Data r7() const;

    [[nodiscard]] Address pc() const;

    void input(Data value);

private:
    bool m_is_halted;

    EmulatorMemory<Address, Data>& m_memory;

    Data m_opcode { 0 };
    Address m_pc;
    Data m_r0 { 0 };
    Data m_r1 { 0 };
    Data m_r2 { 0 };
    Data m_r3 { 0 };
    Data m_r4 { 0 };
    Data m_r5 { 0 };
    Data m_r6 { 0 };
    Data m_r7 { 0 };
    std::stack<Data> m_stack;

    std::vector<OutObserver*> m_out_observers;
    std::vector<InObserver*> m_in_observers;

    Data get_next_value();

    void notify_out_observers(Data acc_reg);

    void notify_in_observers();

    void print_debug(Data opcode);
};
}
