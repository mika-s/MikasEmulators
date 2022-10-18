#ifndef MIKA_EMULATORS_CHIPS_8080_CPU_H
#define MIKA_EMULATORS_CHIPS_8080_CPU_H

#include <cstdint>
#include <vector>
#include "flags.h"
#include "emulator_memory.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/misc/next_word.h"

namespace emu::i8080 {

    using emu::misc::NextByte;
    using emu::misc::NextWord;

    class Cpu {
    public:
        Cpu(
                EmulatorMemory &memory,
                u16 initial_pc
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

        [[nodiscard]] u8 b() const;

        [[nodiscard]] u8 c() const;

        [[nodiscard]] u8 d() const;

        [[nodiscard]] u8 e() const;

        [[nodiscard]] u8 h() const;

        [[nodiscard]] u8 l() const;

        [[nodiscard]] u8 f() const;

        [[nodiscard]] u16 pc() const;

        [[nodiscard]] u16 sp() const;

        [[nodiscard]] bool is_inta() const;

        [[nodiscard]] bool is_interrupted() const;

        void interrupt(u8 supplied_instruction_from_interruptor);

        void input(u8 port, u8 value);

    private:
        static constexpr unsigned int number_of_io_ports = 256;

        bool m_is_halted;

        bool m_inte;
        bool m_is_interrupted;
        u8 m_instruction_from_interruptor;

        EmulatorMemory &m_memory;
        std::size_t m_memory_size;

        std::vector<u8> m_io_in;
        std::vector<u8> m_io_out;

        u8 m_opcode;
        u16 m_sp;
        u16 m_pc;
        u8 m_acc_reg;
        u8 m_b_reg;
        u8 m_c_reg;
        u8 m_d_reg;
        u8 m_e_reg;
        u8 m_h_reg;
        u8 m_l_reg;
        Flags m_flag_reg;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        NextByte get_next_byte();

        NextWord get_next_word();

        void notify_out_observers(u8 port);

        void notify_in_observers(u8 port);

        [[nodiscard]] u16 address_in_HL() const;

        void print_debug();
    };
}

#endif //MIKA_EMULATORS_CHIPS_8080_CPU_H
