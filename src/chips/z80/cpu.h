#ifndef MIKA_EMULATORS_CHIPS_Z80_CPU_H
#define MIKA_EMULATORS_CHIPS_Z80_CPU_H

#include <vector>
#include "flags.h"
#include "emulator_memory.h"
#include "interrupt_mode.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/misc/next_word.h"

namespace emu::z80 {

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

        [[nodiscard]] u8 a_p() const;

        [[nodiscard]] u8 b() const;

        [[nodiscard]] u8 b_p() const;

        [[nodiscard]] u8 c() const;

        [[nodiscard]] u8 c_p() const;

        [[nodiscard]] u8 d() const;

        [[nodiscard]] u8 d_p() const;

        [[nodiscard]] u8 e() const;

        [[nodiscard]] u8 e_p() const;

        [[nodiscard]] u8 h() const;

        [[nodiscard]] u8 h_p() const;

        [[nodiscard]] u8 l() const;

        [[nodiscard]] u8 l_p() const;

        [[nodiscard]] u8 f() const;

        [[nodiscard]] u8 f_p() const;

        [[nodiscard]] u16 ix() const;

        [[nodiscard]] u16 iy() const;

        [[nodiscard]] u8 i() const;

        [[nodiscard]] u8 r() const;

        [[nodiscard]] u16 pc() const;

        [[nodiscard]] u16 sp() const;

        [[nodiscard]] bool is_inta() const;

        [[nodiscard]] bool is_interrupted() const;

        [[nodiscard]] InterruptMode interrupt_mode() const;

        void interrupt(u8 supplied_instruction_from_interruptor);

        void nmi_interrupt();

        void input(u8 port, u8 value);

    private:
        static constexpr unsigned int number_of_io_ports = 256;

        bool m_is_halted;

        bool m_iff1;
        bool m_iff2;
        bool m_is_interrupted;
        bool m_is_nmi_interrupted;
        u8 m_instruction_from_interruptor;

        EmulatorMemory &m_memory;
        std::size_t m_memory_size;

        std::vector<u8> m_io_in;
        std::vector<u8> m_io_out;

        u8 m_opcode;
        u16 m_sp;
        u16 m_pc;
        u8 m_acc_reg;
        u8 m_acc_p_reg;
        u8 m_b_reg;
        u8 m_b_p_reg;
        u8 m_c_reg;
        u8 m_c_p_reg;
        u8 m_d_reg;
        u8 m_d_p_reg;
        u8 m_e_reg;
        u8 m_e_p_reg;
        u8 m_h_reg;
        u8 m_h_p_reg;
        u8 m_l_reg;
        u8 m_l_p_reg;
        u16 m_ix_reg;
        u16 m_iy_reg;
        u8 m_i_reg;
        u8 m_r_reg;
        Flags m_flag_reg;
        Flags m_flag_p_reg;
        InterruptMode m_interrupt_mode;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        void next_bits_instruction(u8 bits_opcode, cyc &cycles);

        void next_ixy_instruction(u8 ixy_opcode, u16 &ixy_reg, cyc &cycles);

        void next_ixy_bits_instruction(NextWord args, u16 &ixy_reg, cyc &cycles);

        void next_extd_instruction(u8 extd_opcode, cyc &cycles);

        cyc handle_nonmaskable_interrupt(cyc cycles);

        cyc handle_maskable_interrupt_0(cyc cycles);

        cyc handle_maskable_interrupt_1_2(cyc cycles);

        NextByte get_next_byte();

        NextWord get_next_word();

        void notify_out_observers(u8 port);

        void notify_in_observers(u8 port);

        [[nodiscard]] u16 address_in_HL() const;

        void r_tick();

        void print_debug(u8 opcode);
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_CPU_H
