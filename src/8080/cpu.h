#ifndef MIKA_EMULATORS_8080_CPU_H
#define MIKA_EMULATORS_8080_CPU_H

#include <cstdint>
#include <vector>
#include "emulator_memory.h"
#include "flags.h"
#include "in_observer.h"
#include "next_word.h"
#include "next_byte.h"
#include "out_observer.h"

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

        emu::cpu8080::EmulatorMemory &Memory();

        [[nodiscard]] std::uint8_t A() const;

        [[nodiscard]] std::uint8_t B() const;

        [[nodiscard]] std::uint8_t C() const;

        [[nodiscard]] std::uint8_t D() const;

        [[nodiscard]] std::uint8_t E() const;

        [[nodiscard]] std::uint8_t H() const;

        [[nodiscard]] std::uint8_t L() const;

        [[nodiscard]] std::uint16_t PC() const;

        [[nodiscard]] bool is_inta() const;

        void interrupt(std::uint8_t supplied_instruction_from_interruptor);

        void input(int port, std::uint8_t value);

    private:
        static constexpr unsigned int number_of_io_ports = 256;

        bool stopped;

        bool inte;
        bool is_interrupted;
        std::uint8_t instruction_from_interruptor;

        emu::cpu8080::EmulatorMemory &memory;
        std::size_t memory_size;

        std::vector<std::uint8_t> io_in;
        std::vector<std::uint8_t> io_out;

        std::uint8_t opcode;
        std::uint16_t sp;
        std::uint16_t pc;
        std::uint8_t acc_reg;
        std::uint8_t b_reg;
        std::uint8_t c_reg;
        std::uint8_t d_reg;
        std::uint8_t e_reg;
        std::uint8_t h_reg;
        std::uint8_t l_reg;
        Flags flag_reg;

        NextByte get_next_byte();

        NextWord get_next_word();

        std::vector<OutObserver *> out_observers;

        std::vector<InObserver *> in_observers;

        void notify_out_observers(std::uint8_t port);

        void notify_in_observers(std::uint8_t port);

        [[nodiscard]] std::uint16_t address_in_HL() const;

        void print_debug();
    };
}

#endif //MIKA_EMULATORS_8080_CPU_H
