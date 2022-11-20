#ifndef MIKA_EMULATORS_CHIPS_LMC_CPU_H
#define MIKA_EMULATORS_CHIPS_LMC_CPU_H

#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <vector>

namespace emu::memory {
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
                EmulatorMemory &memory,
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

        EmulatorMemory &memory();

        [[nodiscard]] u8 a() const;

        [[nodiscard]] u8 pc() const;

        [[nodiscard]] u8 sp() const;

        void input(u8 port, u8 value);

    private:
        bool m_is_halted;

        EmulatorMemory &m_memory;
        std::size_t m_memory_size;

        std::vector<u8> m_io_in;
        std::vector<u8> m_io_out;

        u8 m_opcode;
        u16 m_sp;
        u16 m_pc;
        u8 m_acc_reg;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        NextByte get_next_byte();

        NextWord get_next_word();

        void notify_out_observers(u8 port);

        void notify_in_observers(u8 port);

        void print_debug(u8 opcode);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_CPU_H
