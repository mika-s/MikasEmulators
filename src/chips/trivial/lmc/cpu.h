#ifndef MIKA_EMULATORS_CHIPS_LMC_CPU_H
#define MIKA_EMULATORS_CHIPS_LMC_CPU_H

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "flags.h"
#include <cstddef>
#include <vector>

namespace emu::memory {
    template<class A, class D>
    class EmulatorMemory;
}
namespace emu::lmc {
    class InObserver;
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
                EmulatorMemory<Address, Data> &memory,
                Address initial_pc
        );

        ~Cpu();

        [[nodiscard]] bool can_run_next_instruction() const;

        void next_instruction();

        void reset_state();

        void start();

        void stop();

        void add_out_observer(OutObserver &observer);

        void remove_out_observer(OutObserver *observer);

        void add_in_observer(InObserver &observer);

        void remove_in_observer(InObserver *observer);

        EmulatorMemory<Address, Data> &memory();

        [[nodiscard]] Data a() const;

        [[nodiscard]] Address pc() const;

        void input(Data value);

    private:
        bool m_is_halted;

        EmulatorMemory<Address, Data> &m_memory;
        std::size_t m_memory_size;

        Address m_pc;
        Data m_acc_reg;
        Flags m_flag_reg;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        Data get_next_value();

        void notify_out_observers(Data acc_reg);

        void notify_in_observers();

        void print_debug(Data opcode);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_CPU_H
