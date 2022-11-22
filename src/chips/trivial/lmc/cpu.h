#ifndef MIKA_EMULATORS_CHIPS_LMC_CPU_H
#define MIKA_EMULATORS_CHIPS_LMC_CPU_H

#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
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
                EmulatorMemory<u8, u16> &memory,
                u8 initial_pc
        );

        ~Cpu();

        [[nodiscard]] bool can_run_next_instruction() const;

        cyc next_instruction();

        void reset_state();

        void start();

        void stop();

        void add_out_observer(OutObserver &observer);

        void remove_out_observer(OutObserver *observer);

        void add_in_observer(InObserver &observer);

        void remove_in_observer(InObserver *observer);

        EmulatorMemory<u8, u16> &memory();

        [[nodiscard]] u16 a() const;

        [[nodiscard]] u8 pc() const;

        void input(u16 value);

    private:
        bool m_is_halted;

        EmulatorMemory<u8, u16> &m_memory;
        std::size_t m_memory_size;

        std::vector<u8> m_io_in;
        std::vector<u8> m_io_out;

        u8 m_pc;
        u16 m_acc_reg;
        Flags m_flag_reg;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        u16 get_next_value();

        void notify_out_observers(u16 acc_reg);

        void notify_in_observers();

        void print_debug(u16 opcode);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_CPU_H
