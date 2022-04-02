#ifndef MIKA_EMULATORS_8080_CPU_H
#define MIKA_EMULATORS_8080_CPU_H

#include <cstdint>
#include <vector>
#include "8080/flags.h"
#include "8080/next_word.h"
#include "8080/next_byte.h"
#include "8080/emulator_memory.h"
#include "8080/interfaces/in_observer.h"
#include "8080/interfaces/out_observer.h"

namespace emu::cpu8080 {

    class Cpu {
    public:
        Cpu(emu::cpu8080::EmulatorMemory &memory, std::uint16_t initial_pc);

        ~Cpu();

        [[nodiscard]] bool can_run_next_instruction() const;

        unsigned long next_instruction();

        void reset_state();

        void start();

        void stop();

        void add_out_observer(OutObserver &observer);

        void remove_out_observer(OutObserver *observer);

        void add_in_observer(InObserver &observer);

        void remove_in_observer(InObserver *observer);

        emu::cpu8080::EmulatorMemory &memory();

        [[nodiscard]] std::uint8_t a() const;

        [[nodiscard]] std::uint8_t b() const;

        [[nodiscard]] std::uint8_t c() const;

        [[nodiscard]] std::uint8_t d() const;

        [[nodiscard]] std::uint8_t e() const;

        [[nodiscard]] std::uint8_t h() const;

        [[nodiscard]] std::uint8_t l() const;

        [[nodiscard]] std::uint8_t f() const;

        [[nodiscard]] std::uint16_t pc() const;

        [[nodiscard]] std::uint16_t sp() const;

        [[nodiscard]] bool is_inta() const;

        [[nodiscard]] bool is_interrupted() const;

        void interrupt(std::uint8_t supplied_instruction_from_interruptor);

        void input(int port, std::uint8_t value);

    private:
        static constexpr unsigned int number_of_io_ports = 256;

        bool m_is_stopped;

        bool m_inte;
        bool m_is_interrupted;
        std::uint8_t m_instruction_from_interruptor;

        emu::cpu8080::EmulatorMemory &m_memory;
        std::size_t m_memory_size;

        std::vector<std::uint8_t> m_io_in;
        std::vector<std::uint8_t> m_io_out;

        std::uint8_t m_opcode;
        std::uint16_t m_sp;
        std::uint16_t m_pc;
        std::uint8_t m_acc_reg;
        std::uint8_t m_b_reg;
        std::uint8_t m_c_reg;
        std::uint8_t m_d_reg;
        std::uint8_t m_e_reg;
        std::uint8_t m_h_reg;
        std::uint8_t m_l_reg;
        Flags m_flag_reg;

        std::vector<OutObserver *> m_out_observers;
        std::vector<InObserver *> m_in_observers;

        NextByte get_next_byte();

        NextWord get_next_word();

        void notify_out_observers(std::uint8_t port);

        void notify_in_observers(std::uint8_t port);

        [[nodiscard]] std::uint16_t address_in_HL() const;

        void print_debug();
    };
}

#endif //MIKA_EMULATORS_8080_CPU_H
